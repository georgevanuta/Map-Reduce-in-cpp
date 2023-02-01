# *BONUS*: General Map-Reduce in c++ using Pthreads

`Name: Vănuță George-Andrei`
`Group: 334CB`

## Table of Contents

- [*BONUS*: General Map-Reduce in c++ using Pthreads](#bonus-general-map-reduce-in-c-using-pthreads)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
    - [General *Mappers*](#general-mappers)
    - [General *Reducers*](#general-reducers)

## Overview

- The generalality comes from using `template structs` from `c++`.
As such, in order to solve a Map-Reduce problem, we first need to
establish what structure to use for the *mappers*. This structure is
shared between **ALL** *mappers* by reference (`T *result`).
- After all the *mappers* are finished, the *reducers* need to see what the
*mappers* have done, so they also have access to this field via
`T *fromMappers`.

### General *Mappers*

- The strucure of a *mapper* is of type `mapperArg_t<T>` containing the
following fields:
  - `unsigned int mId;`  $\leftarrow$ the *mapper* id.
  - `std::vector<std::string> filePaths`  $\leftarrow$ all of the input files.
  - `pthread_barrier_t *reducerBarrier`  $\leftarrow$ reference to a barrier
used by all threads in order to let the *mappers* go before the *reducers*.
  - `unsigned int nrReducers` $\leftarrow$ the *mappers* sometimes have to
to know how many *reducers* there are. In case they don't have to, this
field is implicitly set to 0.
  - `T *result`  $\leftarrow$ this is where the generality comes from. The result of a
*mapper* can be of any type, depending on the problem solved.
For example, for the word count problem `T` could be of type
`std::map<int, std::map<std::string, std::int>>`, which maps a *mapper* id to
a map between strings and their ocurrances found by the respective *mapper*.
  - `std::vector<bool> *permissionToFiles` $\leftarrow$ explained in the [README.md](./README.md) file.
  - `pthread_mutex_t *permissionLock` $\leftarrow$ same as above.

### General *Reducers*

- The structure of a *reducer* is of type `reducerArg_t<T>` containing the
following fields:
  - `unsigned int rId` $\leftarrow$ the *reducer* id.
  - `pthread_barrier_t *reducerBarrier` $\leftarrow$ same as in the previous section.
  - `T *fromMappers` $\leftarrow$ reference to the result of the *mappers*.
