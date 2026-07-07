# Routing Table Data Processing Engine

This repository contains a modular C++ application designed to ingest, parse, and query complex network routing table datasets. The project layers a custom-built processing engine, file stream tokenizer, and an interactive multi-tier command-line interface on top of an external data-handling and graphical framework.

> ⚠️ **Implementation Notice:** The core logic, terminal interface, file stream parser, and lambda query mechanisms are contained entirely within the **`Semestralka`** directory, which represents my explicit contribution to this repository. The surrounding `DataStructures` and `Gui` modules serve as external core library dependencies.

---

## 📁 Repository Architecture & Code Ownership

The project workspace is divided cleanly between the application runtime layer and the underlying foundation framework:

├── DataStructures.sln          # Master Visual Studio Solution File
├── Semestralka/                # [MY CORE IMPLEMENTATION] Main Application Module
│   ├── main.cpp                # App entry point & initialization sequence
│   ├── terminal.h              # Multi-tier interactive console IO loop
│   ├── file_reader.h           # Stream-based CSV dataset tokenizer
│   ├── predicates.h            # Bitwise IPv4 translation & time parsing utilities
│   ├── comparators.h           # Ordering configurations (Prefix weights / Lifespans)
│   ├── algorithm.h             # Lambda evaluation engine running over iterators
│   ├── node.h                  # Domain object model for routing table records
│   └── iterator_manager.h      # Contextual state tracker for tree structures
│
├── DataStructures/             # [EXTERNAL DEPENDENCY] Foundation Collection Library
│   # Contains ImplicitSequence, MultiWayExplicitHierarchy, and SortedSequenceTable
└── Gui/                        # [EXTERNAL DEPENDENCY] Supporting User Interface Components


---

## 🚀 Core Implementation Features (Inside `Semestralka/`)

* **Multi-Tier Interactive CLI:** A dynamic, console-driven command shell routing three discrete lookup environments for flexible real-time operations.
* **Extensible Lambda Predicates:** A generic query pipeline passing forward iterators and lambda logic blocks. New network validation criteria can be dropped in dynamically without modifying the engine.
* **Bitwise Subnet Ingestion:** Converts text-encoded IPv4 elements into compact 32-bit binary boundaries (`uint32_t`) to execute immediate bitwise subnet mask evaluations.
* **Chained Collision Management:** Resolves target duplicate matches cleanly by implementing memory-linked routing chains directly attached to shared `NextHop` nodes.
* **Time-Interval Normalization:** Translates varied configurations (like plain seconds, standard `HH:MM:SS`, or mixed tokens like `2h30m`) into plain integers for normalized, high-speed sorting.

---

## 📊 Application Processing Tiers

The application code inside `Semestralka/` leverages the underlying data library to organize data layouts into three progressive tiers:

### Tier 1: Contiguous Sequence Filtering
Performs linear queries across sequential memory layouts (`ImplicitSequence`). Users can filter entire routing records using dynamic conditions (e.g., active lifespan ranges) and sort the live output based on prefix lengths or normalized time seconds.

### Tier 2: Hierarchical Prefix Navigation
Organizes routes into a multi-way prefix tree (Trie Structure) by breaking down IPv4 addresses into 4 distinct byte-octet layers. The CLI lets users browse deep into child branches, return to parent frames, or scope queries strictly to explicit sub-hierarchies.

### Tier 3: Indexed Gateway Lookups
Utilizes an ordered sequence table (`SortedSequenceTable`) keyed directly by gateway IPs. It performs fast binary searches to immediately pinpoint records matching specific `NextHop` keys while tracking overlapping gateway chains.

---

## ⚙️ Compilation & Setup

The solution is designed for **C++17** or newer standards using Microsoft Visual Studio or standard native CLI build environments (GCC/Clang).

### Option 1: Compiling via Visual Studio
1. Open the solution file `DataStructures.sln` inside Visual Studio.
2. In the Solution Explorer, ensure that the **`Semestralka`** project is set as the startup project.
3. Configure your build target to **Release** and architecture to **x64**.
4. Press `Ctrl + Shift + B` to build the binary, and `F5` to execute the console shell.

### Option 2: Compiling via Command Line (GCC)
To compile the application module directly using native tooling, navigate to the project directory, verify your library links, and run:
```bash
g++ -std=c++17 -O3 Semestralka/main.cpp -o routing_processor

Make sure your target dataset file (RT.csv) is located inside your active execution directory before running the compiled application binary.