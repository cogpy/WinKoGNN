"""Setup script for OpenCog Workbench"""

from setuptools import setup, find_packages
import os

# Read README for long description
with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

# Read requirements
with open("requirements.txt", "r", encoding="utf-8") as fh:
    requirements = [line.strip() for line in fh if line.strip() and not line.startswith("#")]

setup(
    name="opencog-workbench",
    version="1.0.0",
    author="CogPy Team",
    author_email="team@cogpy.org",
    description="Autonomous Multi-Agent Orchestration for Windows NT4 Cognitive Kernel",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/cogpy/WinKoGNN",
    packages=find_packages(),
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "Topic :: Scientific/Engineering :: Artificial Intelligence",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Operating System :: Microsoft :: Windows :: NT",
    ],
    python_requires=">=3.7",
    install_requires=requirements,
    extras_require={
        "dev": [
            "pytest>=6.0",
            "pytest-cov>=2.10",
            "black>=20.8b1",
            "flake8>=3.8",
        ],
        "viz": [
            "matplotlib>=3.3.0",
            "networkx>=2.5",
        ],
    },
    entry_points={
        "console_scripts": [
            "opencog-workbench=opencog_workbench.cli:main",
        ],
    },
    keywords="opencog, agent-zero, graph-neural-networks, windows-nt4, cognitive-architecture, multi-agent",
    project_urls={
        "Bug Reports": "https://github.com/cogpy/WinKoGNN/issues",
        "Source": "https://github.com/cogpy/WinKoGNN",
    },
)
