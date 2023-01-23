# Map-Reduce in c++ using Pthreads

`Name: Vănuță George-Andrei`
`Group: 334CB`

## Table of Contents

- [Map-Reduce in c++ using Pthreads](#map-reduce-in-c-using-pthreads)
  - [Table of Contents](#table-of-contents)
  - [How to run?](#how-to-run)
  - [Perfect Numbers](#perfect-numbers)
  - [Map-Reduce](#map-reduce)
    - [*Mappers*](#mappers)
    - [*Reducers*](#reducers)
  - [Synchronisation](#synchronisation)
    - [*Barrier*](#barrier)
    - [*Mutex*](#mutex)
  - [Score Output](#score-output)

## How to run?

> \> make
> \> ./tema1 <NR_MAPPERS> <NR_REDUCERS> <TEST_FILE_PATH>
> \> make clean

## Perfect Numbers

- The *function* which checks if a given *number* is perfect with respect to a
given *exponent* is `helpers::isPerfectNumber`.
- This *function* does *binary search* between `2` and `number / exp`.

## Map-Reduce

- *Mappers* and *reducers* are created at the same time.

### *Mappers*

- The *mappers* will start their work as soon as they are created. This is done
via waiting on the *barrier* after processing the files.
- They will dynamically check for the first availabe input file. If they find one,
they will mark it as visited (`permissionFiles[i] = false`), and then start
processing it. They will put the *perfect numbers* in the same resource
(`mapperArg.result`) shared between all threads which will not create a
race condition, because the resource is a map, which has a special spot
(*key* is the current *mapper id*).
- The resource in which they will put their work is a reference to a map
(`mapperArg.result`) of type `std::map<int, std::map<int, std::vector<long long>>>`.
The first (key, value) pair is a map between a *mapper id* (`mapperArg.mId`) and another
map which maps between powers and their respective *perfect numbers* found by
the current mapper.

### *Reducers*

- The *reducers* will start their work only after the mappers have finished theirs.
This is done via waiting on the *barrier* before reducing the results of the *mappers*.
- They will iterate through all of the entries of the mappers, collecting in a *set*
the perfect numbers of their power (`reducerArg.rId + 2`). The *set* (`individualPerfects`)
is used so that there are no duplicates.
- After that, they will simply output the size of the *set* to a file named
`out{reducerArg.rId + 2}.txt`.

## Synchronisation

### *Barrier*

- A **barrier** (`reducerBarrier`) is used in order to let the *mappers* finish their
work before the *reducers* start. This **barrier** is initialized with the total number
of threads (`NR_MAPPERS + NR_REDUCERS`).
- The *mappers* will wait on this **barrier** after finishing their work and the *reducers*
 will wait on it before finishing their work.
- This resource is shared between all *threads* by reference.

### *Mutex*

- The **mutex** (`permissionLock`) has the purpose of not letting two *mappers* access
the same file at a given time. Also, this will guarantee that, after a *mapper* is done
processing a file, no other *mapper* will try to process it again.
- The **mutex**, in combination with a vector of booleans (`permissionFiles`) which tells us if
a file has been processed / is being processed, achieves a dynamic splitting of the files
between the *mappers*, which grants a better **speedup**.
- This resource is shared between all *mappers* by reference.

## Score Output

`Scor scalabilitate: 70/70`
`Scor corectitudine: 50/50`
`Scor total:       120/120`
