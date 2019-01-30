# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
- TBD

## v0.2.0
### Added
- Tests for calculations
### Changed
- Moved driver functions into struct for a cleaner API and namespacing
- Improved tests

## v0.1.0
### Added
- Project setup, including:
  - CMake setup
  - Unit and property-based tests
- Driver library, including:
  - Interface for user-provided I2C read/write callbacks
  - Basic data structure for the driver
  - Definitions for key manufacturer register addresses and default values
  - Calculations, including:
    - Calculate a register address from a provided channel (0-based)
    - Calculate a prescale value from a provided frequency
    - Bitwise operations for changing LED on and off values
  - Write to individual registers and all registers, including:
    - Set chip base PWM frequency
    - Set duty cycle per channel
    - Set duty cycle on all channels
    - Reset driver
    - Wait the correct amount of time (at least 500μs) after enabling the oscillator
  - Public API, including:
    - set frequency
    - set duty cycle
    - channel on/off
    - soft/hard resets
  - Non-destructive (soft) reset
  - Destructive (hard) reset
- Documentation, including:
  - this CHANGELOG
  - [AUTHORS](https://github.com/minnowpod/libpca9685/tree/master/AUTHORS)
  - [CONDUCT](https://github.com/minnowpod/libpca9685/tree/master/CONDUCT.md)
  - [CONTRIBUTING](https://github.com/minnowpod/libpca9685/tree/master/CONTRIBUTING.md)
  - [LICENSE](https://github.com/minnowpod/libpca9685/tree/master/LICENSE)
  - [README](https://github.com/minnowpod/libpca9685/tree/master/README.md)
