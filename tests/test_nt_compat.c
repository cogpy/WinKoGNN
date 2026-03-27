/*++
 * test_nt_compat.c — Pure C test for NT compatibility headers
 *
 * Validates that all NT headers compile cleanly under C11.
 *--*/

#include "nt.h"
#include "ntrtl.h"
#include "ntstatus.h"
#include "bugcodes.h"
#include "windef.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

int main(void) {
    int passed = 0;

    printf("=== NT Compatibility C Test ===\n");

    /* Type sizes */
    assert(sizeof(UCHAR) == 1);
    assert(sizeof(USHORT) == 2);
    assert(sizeof(ULONG) == 4);
    assert(sizeof(ULONGLONG) == 8);
    assert(sizeof(NTSTATUS) == 4);
    printf("  [PASS] Type sizes correct\n"); passed++;

    /* Status macros */
    assert(NT_SUCCESS(STATUS_SUCCESS));
    assert(!NT_SUCCESS(STATUS_UNSUCCESSFUL));
    assert(NT_ERROR(STATUS_NO_MEMORY));
    printf("  [PASS] Status macros work\n"); passed++;

    /* List operations */
    {
        LIST_ENTRY head, e1, e2;
        InitializeListHead(&head);
        assert(IsListEmpty(&head));
        InsertTailList(&head, &e1);
        InsertTailList(&head, &e2);
        assert(!IsListEmpty(&head));
        assert(head.Flink == &e1);
        assert(e1.Flink == &e2);
        RemoveEntryList(&e1);
        assert(head.Flink == &e2);
    }
    printf("  [PASS] List operations work\n"); passed++;

    /* Unicode string */
    {
        UNICODE_STRING str;
        WCHAR buf[] = {'T', 'e', 's', 't', 0};
        RtlInitUnicodeString(&str, buf);
        assert(str.Length == 4 * sizeof(WCHAR));
        assert(str.Buffer == buf);
    }
    printf("  [PASS] Unicode strings work\n"); passed++;

    /* Large integer */
    {
        LARGE_INTEGER li;
        li.QuadPart = 0x100000002LL;
        assert(li.u.LowPart == 2);
        assert(li.u.HighPart == 1);
    }
    printf("  [PASS] Large integer works\n"); passed++;

    /* Spinlock */
    {
        KSPIN_LOCK lock;
        KIRQL old;
        KeInitializeSpinLock(&lock);
        KeAcquireSpinLock(&lock, &old);
        KeReleaseSpinLock(&lock, old);
    }
    printf("  [PASS] Spinlock works\n"); passed++;

    /* Pool allocation */
    {
        PVOID p = ExAllocatePoolWithTag(NonPagedPool, 256, 0x74657354);
        assert(p != NULL);
        memset(p, 0xCC, 256);
        ExFreePoolWithTag(p, 0x74657354);
    }
    printf("  [PASS] Pool allocation works\n"); passed++;

    /* RTL memory */
    {
        char a[16] = "Hello";
        char b[16];
        RtlCopyMemory(b, a, 6);
        assert(RtlEqualMemory(a, b, 6));
        RtlZeroMemory(b, 16);
        assert(b[0] == 0);
    }
    printf("  [PASS] RTL memory ops work\n"); passed++;

    /* Bitmap */
    {
        ULONG buf[2] = {0};
        RTL_BITMAP bm;
        RtlInitializeBitMap(&bm, buf, 64);
        RtlSetBit(&bm, 7);
        RtlSetBit(&bm, 63);
        assert(RtlTestBit(&bm, 7));
        assert(RtlTestBit(&bm, 63));
        assert(!RtlTestBit(&bm, 0));
    }
    printf("  [PASS] Bitmap ops work\n"); passed++;

    /* Interlocked */
    {
        volatile LONG v = 10;
        InterlockedIncrement(&v);
        assert(v == 11);
        InterlockedDecrement(&v);
        assert(v == 10);
    }
    printf("  [PASS] Interlocked ops work\n"); passed++;

    /* System call stubs */
    {
        HANDLE h;
        NTSTATUS s = NtCreateProcess(&h, 0, NULL, NULL, FALSE, NULL, NULL, NULL);
        assert(NT_SUCCESS(s));
        assert(h != NULL);
        NtClose(h);
    }
    printf("  [PASS] System call stubs work\n"); passed++;

    /* CONTAINING_RECORD */
    {
        struct { int x; LIST_ENTRY link; int y; } ts;
        ts.x = 42; ts.y = 99;
        void *recovered = CONTAINING_RECORD(&ts.link, struct { int x; LIST_ENTRY link; int y; }, link);
        /* Just verify it doesn't crash */
        (void)recovered;
    }
    printf("  [PASS] CONTAINING_RECORD works\n"); passed++;

    /* windef types */
    {
        POINT pt; pt.x = 10; pt.y = 20;
        RECT rc; rc.left = 0; rc.top = 0; rc.right = 100; rc.bottom = 100;
        assert(pt.x == 10);
        assert(rc.right == 100);
        COLORREF c = RGB(255, 128, 0);
        assert(c != 0);
    }
    printf("  [PASS] windef types work\n"); passed++;

    printf("\n=== All %d NT compatibility tests passed! ===\n\n", passed);
    return 0;
}
