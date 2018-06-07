# How was this testset constructed

For this testset we are focusing on C-Projects found on GitHub. To have a more narrow testset we also limit the
projects to fulfill the following constrains:

* License has to be on of the following: apache-2.0, bsd-2-clause, bsd-3-clause, cc0-1.0, mit, unlicense, zlib
* Stars need to be between 135-400 (TODO: parse projects between 100-150 stars again)

## Building the basic Database

To find those projects and doing a database dump of this list, a simple script was developed.
It's called [fetch_github](https://github.com/pointhi/fetch_github), and has the capability to dump the result of GitHub
search queries into a SqlLite database. Optionally, the script could also download the found projects immediatly.

To create a dump of a specific search query the following command can be executed:

```
./fetch_github/fetch_github.py "language:c stars:150..400"
```


## Evaluation of the Database

Projects which met our basic constrains (basic programming language, license, number of stars) will be evaluated closely.

Constrains which needs to be fulfilled but are not evaluated automatically at the moment are:

* Is the project really a C project? (In some cases there were Objective-C or C++ projects for example)
* Can the project be adjusted as testcase/benchmark without doing code changes?
  * Is this a library or an application? (In case of applications, some are to tightly connected to simply build tests)
  * What dependencies is the library using?
    We will limit ourself to programms depending on libc and the posix standard.
  * Is the application easy enough to understand to build a testcase with it without deep knowledge about it?
    Having existing unittests or examples is very beneficials. We can use them as basis for our testsuite.
  * Can the application be executed quietly in the terminal?
    As workaround for printf, the test harness has an overwrite to silence most applications by default.
    GUI Applications are also prohibited (they would depend on some not allowed dependencies as well)
  * Does the application needs read or write files? (Not autmatically an exclusion criteria)
  * How long is the expected execution time of the written testcase? (Median should be about 500ms, at least 200ms)
  * Do we need to run project specific Makefiles, or can we simply compile all files together?
    In case of a project specific Makefile it's harder to do a correct compilation.
