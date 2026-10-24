# Crypto Matching Engine

A C++20 cryptocurrency matching engine built from scratch as a systems programming and software engineering learning project.

The project models the core behaviour of an electronic exchange: accepting orders, maintaining order books, matching compatible buyers and sellers, generating trades, publishing market data, and processing commands across multiple trading symbols.

Rather than using an existing matching engine or exchange framework, the project has been developed incrementally to understand the underlying data structures, matching rules, ownership decisions, APIs, testing strategies, and system architecture involved in building this type of software.

---

## Project Purpose

The primary purpose of this project is learning.

I built the matching engine to improve my understanding of:

- Modern C++ design
- Order-driven financial markets
- Matching algorithms
- Price-time priority
- Data structures and access patterns
- API design
- Object ownership and lifetimes
- Deterministic testing
- Event-driven architecture
- Market data generation
- Trade publication
- CMake project organisation
- GoogleTest
- Designing a larger C++ system incrementally

The project started with basic types and an order model and has gradually developed into an end-to-end application capable of reading simulated order flow from a file and processing it through a multi-symbol matching engine.

The intention has not been to build a production exchange, but to build enough of one from first principles to understand the engineering problems involved.

---

# What the Engine Does

At a high level:

```text
Order Input
    |
    v
Order Parser
    |
    v
Engine Command
    |
    v
Matching Engine
    |
    +--------------------+
    |                    |
    v                    v
Order Books           Engine Events
    |
    v
Matching
    |
    +--------------------+
    |                    |
    v                    v
Trades              Market Data
```

The current implementation supports multiple trading symbols and maintains an independent order book for each symbol.

Orders can rest on the book, execute against existing liquidity, partially fill, fully fill, or be rejected depending on their properties and time-in-force behaviour.

---

# Current Features

## Order Model

Orders contain the information required by the matching engine, including:

- Order ID
- Symbol
- Side
- Price
- Original quantity
- Remaining quantity
- Market-order state
- Time in force

The order model tracks fills and determines when an order has been completely filled.

---

## Order Validation

Incoming orders are validated before entering the matching process.

Validation includes checks for invalid:

- Order IDs
- Symbols
- Prices
- Quantities

Market orders are handled separately where appropriate because they do not require a conventional limit price.

Rejected orders produce explicit rejection information rather than silently failing.

---

## Price Levels

Orders resting at the same price are grouped into price levels.

Each level maintains its orders in FIFO order, allowing the engine to implement price-time priority.

Price levels support operations including:

- Adding orders
- Removing orders
- Looking up orders
- Inspecting the front order
- Calculating total remaining quantity

---

## Order Book

Each trading symbol has its own order book.

The order book maintains separate bid and ask sides and can determine:

- Best bid
- Best ask
- Available liquidity
- Orders at individual price levels

Bid prices are prioritised from highest to lowest.

Ask prices are prioritised from lowest to highest.

---

# Matching

The engine implements price-time-priority matching.

For compatible orders, price determines priority first and arrival order determines priority between orders at the same price.

The matching logic supports:

- Basic limit-order matching
- Partial fills
- Full fills
- Matching across multiple resting orders
- Matching across multiple price levels

An incoming order can therefore consume liquidity from several existing orders before either being completely filled or resting its remaining quantity on the book.

---

# Market Orders

Market orders are supported.

Unlike limit orders, an unfilled portion of a market order is never allowed to rest on the order book.

The engine attempts to execute the order against currently available opposite-side liquidity and discards any quantity that cannot execute.

---

# Time In Force

The engine supports different order lifetime behaviours.

These include:

### GTC — Good Till Cancelled

Any unfilled quantity can remain resting on the order book.

### IOC — Immediate Or Cancel

The order immediately attempts to match available liquidity.

Any quantity that cannot immediately execute is discarded rather than resting.

### FOK — Fill Or Kill

The engine first determines whether enough compatible liquidity exists to fill the entire order.

If the complete quantity cannot execute, the order is not partially executed.

---

# Order Cancellation

Resting orders can be cancelled using their order ID and symbol.

Successful cancellations remove the order from its price level and remove empty levels where necessary.

Attempts to cancel unknown orders fail without changing the book.

---

# Order Modification

Existing resting orders can be modified.

The engine supports replacing the price and quantity associated with an existing order while maintaining the consistency of the order book.

---

# Trades

Successful matches generate explicit trade objects.

A trade records information including:

- Trade ID
- Buy order ID
- Sell order ID
- Symbol
- Execution price
- Executed quantity

This separates the concept of an order from the execution generated when two orders match.

---

# Trade Publishing

Trade generation is separated from trade consumption through a publisher interface.

The matching engine can publish generated trades without needing to know what ultimately consumes them.

An in-memory trade publisher is currently provided for testing and simulation.

This design makes it possible to later replace the in-memory implementation with another destination such as:

- Logging
- A database
- A network service
- A message queue
- A downstream market-data system

without coupling those systems directly to the matching algorithm.

---

# Multi-Symbol Matching Engine

The matching engine manages multiple independent order books.

For example:

```text
Matching Engine
    |
    +-- BTC-GBP Order Book
    |
    +-- ETH-GBP Order Book
    |
    +-- SOL-GBP Order Book
```

A book is created for a symbol when required.

Commands are routed to the appropriate order book based on their symbol.

---

# Command API

Operations are represented as engine commands rather than directly manipulating order books from external code.

The command model currently supports:

```text
SUBMIT_ORDER
CANCEL_ORDER
MODIFY_ORDER
```

This provides a boundary between external order flow and the internal matching implementation.

---

# Engine Events

The engine can produce events describing the result of commands.

Events include:

```text
ORDER_PROCESSED
ORDER_CANCELLED
ORDER_MODIFIED
COMMAND_REJECTED
```

This separates requesting an operation from observing its result and provides a foundation for a more event-driven system.

---

# Matching Results and Rejections

Order processing produces structured matching results.

Rather than relying only on boolean success/failure values, the engine can represent outcomes such as:

- Resting orders
- Filled orders
- Partially filled orders
- Rejected orders

Rejected orders contain explicit rejection information describing why processing failed.

---

# Market Data

The engine can generate market-data snapshots from its current order-book state.

## Level One

Level-one data includes information such as:

- Best bid
- Best bid quantity
- Best ask
- Best ask quantity

## Level Two

Level-two market data exposes multiple aggregated price levels.

For example:

```text
ASKS

Price       Quantity
50300       15
50200       30
50100       20

---------------------

BIDS

Price       Quantity
50000       25
49900       40
49800       10
```

Orders at the same price are aggregated into a single market-data level.

---

# Sequence Numbers

Order books maintain sequence numbers so consumers can identify changes to book state.

Successful state-changing operations advance the sequence number.

Failed operations and rejected orders do not incorrectly advance it.

Market-data snapshots include the current sequence number.

This provides a foundation for reasoning about ordered market-data updates and deterministic state changes.

---

# Deterministic Clock

Time is abstracted behind a clock interface.

The project provides:

- A system clock for normal execution
- A controllable test clock for deterministic unit tests

Market-data snapshots can therefore contain timestamps without tests depending on the actual system clock.

This makes time-dependent behaviour reproducible.

---

# Test Fixture Builders

Reusable test builders are provided for creating orders.

This reduces duplicated setup code while allowing tests to clearly specify only the properties relevant to the behaviour being tested.

---

# File-Based Order Input

The engine can receive simulated order flow from a text file.

Example:

```text
BUY BTC-GBP 50000 10
BUY BTC-GBP 49900 20
SELL BTC-GBP 50500 15
SELL BTC-GBP 50000 5
CANCEL BTC-GBP 2
```

Input is parsed into engine commands before being passed to the matching engine.

The parser and file reader are deliberately separated so parsing behaviour can be tested independently from file I/O.

---

# Simulated Order Feed

The simulated order feed connects external file input to the matching engine.

The complete flow is:

```text
orders.txt
    |
    v
CMEFileOrderReader
    |
    v
CMEOrderInputParser
    |
    v
CMEEngineCommand
    |
    v
CMESimulatedOrderFeed
    |
    v
CMEMatchingEngine
    |
    +---------------------+
    |                     |
    v                     v
Order Books             Trades
    |
    v
Market Data
```

This provides an end-to-end way of exercising the engine without requiring a live exchange connection or network service.

---

# Testing

The project uses GoogleTest.

At the completion of the 034_simulated-order-feed branch/milestone, the project contains 124 passing unit and integration tests covering the major components of the engine.

Tests cover areas including:

- Core financial types
- Orders
- Validation
- Price levels
- Order books
- Price-time priority
- Partial fills
- Full fills
- Market orders
- IOC orders
- FOK orders
- Cancellation
- Modification
- Rejections
- Matching results
- Trades
- Multi-symbol processing
- Engine commands
- Engine events
- Trade publishing
- Market-data snapshots
- Level-one market data
- Level-two market data
- Sequence numbers
- Deterministic timestamps
- Test fixture builders
- Order-input parsing
- File reading
- Simulated order feeds

The test suite is intended both to verify behaviour and to make refactoring safer as the project develops.

---

# Technology

The project currently uses:

- **C++20**
- **CMake**
- **GoogleTest**
- **STL containers and utilities**
- **Git**
- **GitHub**

The implementation intentionally avoids relying on a specialised trading or matching-engine framework.

The purpose is to understand and implement the underlying concepts directly.

---

# Project Structure

The project is broadly organised as:

```text
model/
software/
|
+-- app/
|   |
|   +-- main.cpp
|
+-- include/cme/
|   |
|   +-- core/
|   +-- engine/
|   +-- input/
|   +-- market_data/
|   +-- matching/
|   +-- order/
|   +-- order_book/
|   +-- time/
|   +-- trade/
|
+-- src/
|   |
|   +-- engine/
|   +-- input/
|   +-- market_data/
|   +-- matching/
|   +-- order/
|   +-- order_book/
|   +-- time/
|   +-- trade/
|
+-- tests/
    |
    +-- core/
    +-- engine/
    +-- fixtures/
    +-- input/
    +-- market_data/
    +-- matching/
    +-- order/
    +-- order_book/
    +-- time/
    +-- trade/
```

Public interfaces live under `software/include/cme`, implementations under `software/src`, and tests are organised by component under `software/tests`.

---

# Building the Project

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

---

# Running the Tests

Run:

```bash
ctest --test-dir build --output-on-failure
```

The project uses GoogleTest discovery through CMake, so individual GoogleTest cases are registered with CTest.

---

# Running the Matching Engine

The application accepts a file containing simulated order input.

For example:

```bash
./build/cme_app orders.txt
```

An example input file might contain:

```text
BUY BTC-GBP 50000 10
SELL BTC-GBP 51000 20
SELL BTC-GBP 50000 5
BUY ETH-GBP 3000 20
SELL ETH-GBP 3000 10
```

The simulated feed parses each line and submits the resulting commands to the matching engine.

---

# Design Principles

Several principles have guided the project.

## Build Incrementally

Features were intentionally implemented in small branches.

For example, the project did not begin with a complete matching engine.

It progressed through:

```text
Types
  ->
Orders
  ->
Validation
  ->
Price Levels
  ->
Order Book
  ->
Matching
  ->
Trades
  ->
Advanced Order Behaviour
  ->
Multi-Symbol Engine
  ->
Commands / Events
  ->
Trade Publishing
  ->
Market Data
  ->
Sequence Numbers / Time
  ->
External Input
```

This made it possible to understand and test each concept independently before building on it.

## Separate Responsibilities

The project attempts to avoid putting every responsibility into the matching engine.

Examples include separate components for:

- Order validation
- Price levels
- Order books
- Matching results
- Engine commands
- Engine events
- Trade publishing
- Market-data snapshots
- Clocks
- Input parsing
- File reading
- Simulated feeds

## Deterministic Behaviour

Matching engines are stateful systems where ordering matters.

Sequence numbers, controlled clocks, FIFO price levels, explicit command processing, and deterministic tests are therefore used to make behaviour easier to reason about and reproduce.

## Test Behaviour, Not Just Compilation

The project contains tests for actual matching behaviour rather than only verifying that individual classes can be constructed.

For example, tests verify:

- FIFO behaviour
- Best bid/ask selection
- Multi-level matching
- Partial fills
- Cancellation
- Modification
- IOC behaviour
- FOK behaviour
- Market-data aggregation
- Sequence-number changes
- Deterministic timestamps
- End-to-end simulated input

---

# What I Have Used This Project For

This repository is primarily a practical learning environment for developing stronger C++ and systems-programming skills.

I have used it to work through questions such as:

- How should an order book represent bid and ask levels?
- Which data structures make best-price lookup straightforward?
- How should FIFO priority be maintained within a price level?
- What happens to an incoming order during a partial fill?
- How should market orders differ from limit orders?
- How can FOK eligibility be determined before mutating the book?
- How should commands be separated from matching logic?
- How should generated trades leave the matching engine?
- How can market data be derived from internal book state?
- How can state changes be sequenced?
- How can time-dependent code remain deterministic in tests?
- How can external input be introduced without coupling file I/O to the engine?

The project has also given me a larger codebase in which to practise reading my own abstractions, debugging interactions between components, evolving APIs, maintaining tests, and reasoning about changes that affect multiple parts of a system.

---

# What This Project Is Not

This is a learning implementation rather than a production cryptocurrency exchange.

It currently does not attempt to provide all of the infrastructure required by a real trading venue, such as:

- Persistent order recovery
- Network gateways
- Authentication
- Risk management
- Account/balance management
- Real exchange connectivity
- Distributed replication
- Production-grade durability
- Production monitoring
- Regulatory functionality
- High-availability failover

The project focuses specifically on understanding the core matching-engine and order-book problem before introducing increasingly complex systems concerns.

---

# Continuing the Project

The repository is intentionally structured so additional systems work can be added incrementally.

A developer picking up the project could experiment with:

- Alternative order-book data structures
- Different allocation strategies
- More detailed market-data events
- Binary input formats
- Persistence
- Networking
- Additional order types
- Latency measurement
- Replay tools
- Exchange gateways
- More sophisticated benchmarking

Because the matching behaviour is covered by an extensive test suite, these areas can be explored while checking that existing behaviour remains intact.

---

# Remaining Roadmap

The next planned work focuses less on adding matching features and more on understanding the engineering concerns that become important around a matching engine.

## `035-logging`

Introduce structured logging around important engine activity while keeping logging separate from core matching decisions.

Areas to explore include:

- Command processing
- Rejections
- Trades
- Errors
- Engine lifecycle events

## `036-error-handling`

Review failure paths across the system and make error handling more consistent and explicit.

This includes distinguishing expected business-level rejection from unexpected application/system failure.

## `037-performance-benchmarking`

Introduce repeatable benchmarks for important engine operations.

Potential measurements include:

- Orders processed per second
- Matching latency
- Book insertion performance
- Cancellation performance
- Different book depths
- Different matching workloads

The purpose is to measure before attempting optimisation.

## `038-memory-ownership-review`

Review ownership and object lifetimes throughout the engine.

This includes examining:

- Raw non-owning pointers
- Object lifetimes
- Copies
- References
- Container ownership
- Potential unnecessary allocations

The aim is to develop a clearer understanding of the memory model of the application before making performance-oriented changes.

## `039-thread-safety-boundaries`

Identify which components own mutable state and define where concurrency should and should not be introduced.

Rather than immediately adding locks throughout the codebase, this branch will establish explicit concurrency boundaries first.

## `040-single-threaded-engine-loop`

Move towards an explicit single-threaded matching-engine processing loop.

The goal is to keep mutation of order-book state deterministic and owned by a single execution context.

This provides a foundation for introducing concurrency around the engine rather than inside the core matching path.

## `041-concurrent-input-queue`

Introduce concurrent command ingestion while preserving single-threaded matching behaviour.

The intended architecture is broadly:

```text
Producer Thread(s)
       |
       v
Concurrent Input Queue
       |
       v
Single Matching Thread
       |
       v
Matching Engine
       |
       +----> Trades
       |
       +----> Market Data
```

This allows concurrency to be explored without immediately making the order books themselves concurrently mutable.

---

# Current Status

The matching-engine feature set through:

```text
034-simulated-order-feed
```

is implemented.

The current system provides an end-to-end path from text-based simulated order input through command parsing and matching to trades and market-data state.

The next planned branch is:

```text
035-logging
```

The remaining roadmap is:

```text
035-logging
036-error-handling
037-performance-benchmarking
038-memory-ownership-review
039-thread-safety-boundaries
040-single-threaded-engine-loop
041-concurrent-input-queue
```

---

## Why This Repository Exists

The goal of this project is not simply to arrive at a finished matching engine.

The value is in understanding how the system works, why individual components exist, how design decisions interact, how behaviour can be tested, and how a relatively small initial C++ program can evolve into a structured system.

It is an ongoing project that I intend to revisit as my understanding of C++, performance engineering, concurrency, and low-latency systems develops.