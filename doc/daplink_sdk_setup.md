# Development Environment Setup

In order to compile build and test DAPLink from the source code you need to install all necessary tools first, so called dependencies, which are:
* [Python 2.7][python] is a platform independent free and open programming language (get the most up tp date version along with `pip` and `virtualenv`).
* [Git][git] is a distributed version control system for collaborative remote source code management.
* [Keil MDK-ARM][uvision] is a commercial high quality Embedded ARM SDK that we use at the moment to build the DAPLink firmwares. Use version 5 as preferred. Set environment variable "UV4" to the absolute path of the UV4 executable if you don't install to the default location. Note that "UV4" is what's used for both MDK versions 4 and 5.

Now in few steps we will create a working directory and set it up to contain the source code from remote git repository along with other necessary components. Maybe we will create an automation script one day that will make it for you, but it is really good to know the tools you are going to work with:

**Step 1:** Get the DAPLink Source Code using GIT:

	$ cd <your_project_location>
	$ git clone https://github.com/mbedmicro/DAPLink DAPLink.git
	$ cd DAPLink.git

**Step 2:** Create a Python Virtual Environment, then activate it:

	$ pip install virtualenv
	$ virtualenv27 venv
	$ source venv/bin/activate

This Virtual Environment is a nice concept to create local copy of whole Python environment to work with, modify packages set, and then remove if necessary, with no impact on system-wide Python installation. In case anything goes wrong we can easily remove and re-create virtual environment without messing up whole operating system. Just remember that you need to `activate` it first, otherwise you will be using defaul system Python interpreter, and `deactivate` after:

	$ which python
	/opt/local/bin/python
	$ python
	Python 3.5.3 (default, Apr 23 2017, 18:09:27)
	[GCC 4.2.1 Compatible Apple LLVM 8.0.0 (clang-800.0.42.1)] on darwin
	Type "help", "copyright", "credits" or "license" for more information.
	>>> ^D>>>
	
	$ source venv/bin/activate
	(venv) $ which python
	/<your_working_directory>/venv/bin/python
	(venv) $ python
	Python 2.7.13 (default, Apr 23 2017, 16:50:35)
	[GCC 4.2.1 Compatible Apple LLVM 8.0.0 (clang-800.0.42.1)] on darwin
	Type "help", "copyright", "credits" or "license" for more information.
	>>>

Depending on the Operating System you are using the names and invocation method of the `activate` and `deactivate` scripts may differ:
* `source venv/bin/activate` (use for Linux/Unix bash).
* `venv/Scripts/activate` (use for Windows).

**Step 3:** Update Python tools and install required libraries/packages: 

	$ pip install -r requirements.txt
	
**Step 4:** Generate set of Keil uVision project files for each supported variation of DAPLink firmware. **This should be done every time you pull new changes from a git repository!**

	$ progen generate -t uvision
	

**Step 5.** You are ready to make your own work now. If you intend to make modifications to the project, please create your own [Fork at GitHub][github_fork], then commit local changes, push them to your fork, then [make a Pull Request to the Upstream project][github_pullrequest]. Pull requests should be made once a changeset is [rebased onto Master][git_merge_vs_rebase].

**Note:** We are currently using Keil uVision MDK 5 to build DAPLink firmware because of high quality of the produced code and small footprint of the binary.

[python]: https://www.python.org/downloads/ "Python is platform independent free and open programming language"
[git]: https://git-scm.com/downloads "GIT is a distributed version control system"
[uvision]: http://www2.keil.com/mdk5/uvision/ "Keil uVision IDE"
[git_merge_vs_rebase]: https://www.atlassian.com/git/tutorials/merging-vs-rebasing/workflow-walkthrough "Article on GIT Merge vs Rebase operations"
[github_fork]: https://help.github.com/articles/fork-a-repo/ "How to Fork a Project on GitHub"
[github_pullrequest]: https://help.github.com/articles/about-pull-requests/ "About Pull Request on GitHub"