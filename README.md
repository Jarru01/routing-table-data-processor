# routing-table-data-processor

A C++17 console app that loads an IPv4 routing table export and lets you query it in three
ways: filter the full list, browse routes in a tree keyed by address octets, or look up
every route that uses a given next hop.

My code is in [`Semestralka/`](Semestralka). [`DataStructures/`](DataStructures) and
[`Gui/`](Gui) are a provided data structures library (ADTs, memory types, unit tests and a
test GUI) that the app is built on; I didn't write them.

## What it does

The app reads the routing table once and builds three structures from it. Each one has its own
menu:

1. **Filter the list** (`ImplicitSequence`). Pick a filter, then optionally sort the result.
   - *Address match*: routes whose network contains the IP you enter. The address and its
     CIDR mask are converted to 32-bit integers and compared bitwise.
   - *Lifetime range*: routes whose age is within a range such as `2d3h-1w`. Lifetimes can be
     written as `1w2d`, `3d04h` or `hh:mm:ss`.
   - *Next hop*: routes that go via a given gateway.
   - Sorting uses quicksort with a lambda comparator: by prefix (network, then mask length) or
     by lifetime.
2. **Browse the octet tree** (`MultiWayExplicitHierarchy`). Routes are stored in a four-level
   tree, one level per octet (`10` → `10.1` → `10.1.5` → `10.1.5.0`). You can move down to a
   child or back up, and run the same filters on just the current subtree.
3. **Next-hop lookup** (`SortedSequenceTable`). A table keyed by next hop; binary search finds
   the first route, and further routes with the same next hop are chained to it.

The filters are written once as a generic function that takes an iterator range, a predicate
lambda and an action lambda ([`algorithm.h`](Semestralka/algorithm.h)), so the same code runs
over the flat list and over a tree subtree.

The console menus are in Slovak.

## Input format

A `;`-separated file with a header line and six columns:

```text
source;lifetime;address;mask;metric;next-hop
O;1w2d;10.1.0.0;16;[110/2];via 192.0.2.1
S;02:15:30;192.168.10.0;24;[1/0];via 198.51.100.1
```

The app uses `lifetime`, `address`, `mask` (prefix length) and `next-hop`. An empty mask is
treated as `/32`. [`data/sample-rt.csv`](data/sample-rt.csv) is a small sample with
documentation and private addresses.

## Build and run

Windows with Visual Studio 2022 (the "Desktop development with C++" workload):

1. Open `DataStructures.sln`.
2. Set **Semestralka** as the startup project and select **Release | x64**.
3. Build with `Ctrl+Shift+B`.

Pass the dataset path as the first argument. Without one, the app looks for `RT.csv` in the
current directory:

```bat
x64\Release\Semestralka.exe data\sample-rt.csv
```

## Project layout

| Path | Contents |
| :-- | :-- |
| `Semestralka/main.cpp` | Loads the data and runs the main menu |
| `Semestralka/file_reader.h` | CSV parsing and building the list, octet tree and next-hop table |
| `Semestralka/terminal.h` | The three interactive menus |
| `Semestralka/algorithm.h` | Generic filter over an iterator range with predicate and action lambdas |
| `Semestralka/predicates.h` | Address, lifetime and next-hop filters |
| `Semestralka/comparators.h` | Sort orders (prefix, lifetime) |
| `Semestralka/iterator_manager.h` | Moving through the octet tree and pre-order traversal |
| `Semestralka/node.h` | A routing table entry |
| `DataStructures/`, `Gui/` | The provided library, its tests and test GUI |
