#pragma once

enum class OptionExpiryType {
    European,
    American,
    Bermuda
};

enum class FiniteDifferenceType {
    Explicit, 
    Implicit, 
    CrankNicolson
};