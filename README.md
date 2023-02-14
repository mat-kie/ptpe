# ptpe

Partial implementation of the precision time protocol (IEEE1588-2019) targeting embedded systems.

currently only the bare minimum E2E two step ordinary clock functionality is implemented. This is an early version which is by no means compliant to the IEEE1588-2019 standard (yet).

## goals

- no external dependencies
- lean interface for platform specific code
- no dynamic memory allocation
- mimic the structure and naming from the standard.

## Interface

- define your compiler specific directive for packed structs in include/ptpe/interface/platform.hpp.
- implement the Transport and ClockServo concepts in include/ptpe/interface/concepts.hpp.
- all possible extension points are defined as concepts in include/ptpe/interface/concepts.hpp.