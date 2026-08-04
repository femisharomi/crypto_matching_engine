#ifndef CME_CORE_TYPES_HPP
#define CME_CORE_TYPES_HPP

#include <cstdint>
#include <string>

/* Represents which side of the market an order belongs to. */
enum class CMESide
{
    BUY,
    SELL,
    UNKNOWN
};

/* Represents how long an order is allowed to remain active. */
enum class CMETimeInForce
{
    GTC, // GTC (Good 'Til Cancelled): Remains active indefinitely until the order fills or you manually cancel it 
    IOC, // IOC (Immediate-Or-Cancel): Requires all or part of the order to execute immediately; any unfilled portion is instantly cancelled.
    FOK // FOK (Fill-Or-Kill): Requires the entire order to execute immediately and completely, or else the whole order is cancelled (no partial fills).
};

/* Represents a price. */
struct CMEPrice
{
    // Variables
    std::int64_t value;

    // Functions
    // Constructor that initialises the struct's value member
    CMEPrice(std::int64_t initial_value) : value(initial_value) 
    {
    }

    // Checks if two prices are exactly the same
    bool operator== (const CMEPrice &rhs) const
    {
        return value == rhs.value;
    }
    
    // Checks if two prices are different
    bool operator!= (const CMEPrice &rhs) const
    {
        return (value != rhs.value);
    }
    
    // Checks if this price is cheaper than the other price
    bool operator< (const CMEPrice &rhs) const
    {
        return (value < rhs.value);
    }
    
    // Checks if this price is more expensive than the other price
    bool operator> (const CMEPrice &rhs) const
    {
        return (value > rhs.value);
    }

    // Checks if this price is cheaper than or equal to the other price
    bool operator<= (const CMEPrice &rhs) const
    {
        return (value <= rhs.value);
    }

    // Checks if this price is more expensive than or equal to the other price
    bool operator>= (const CMEPrice &rhs) const
    {
        return (value >= rhs.value);
    }
};

/* Represents how much of an asset is being traded. */
struct CMEQuantity
{
    // Variables
    std::int64_t value;

    // Functions
    // Constructor that initialises the struct's value member
    CMEQuantity(std::int64_t initial_value) : value(initial_value) 
    {
    }

    // Checks if two quantities are exactly the same
    bool operator== (const CMEQuantity &rhs) const
    {
        return value == rhs.value;
    }
    
    // Checks if two quantities are different
    bool operator!= (const CMEQuantity &rhs) const
    {
        return (value != rhs.value);
    }
    
    // Checks if this quantity is less than the other quantity
    bool operator< (const CMEQuantity &rhs) const
    {
        return (value < rhs.value);
    }
    
    // Checks if this quantity is more than the other quantity
    bool operator> (const CMEQuantity &rhs) const
    {
        return (value > rhs.value);
    }

    // Checks if this quantity is less than or equal to the other quantity
    bool operator<= (const CMEQuantity &rhs) const
    {
        return (value <= rhs.value);
    }

    // Checks if this quantity is more than or equal to the other quantity
    bool operator>= (const CMEQuantity &rhs) const
    {
        return (value >= rhs.value);
    }
};

/* Represent an order's unique identifier. */
struct CMEOrderId
{
    // Variables
    std::uint64_t value;

    // Functions
    // Constructor that initialises the struct's value member
    CMEOrderId(std::uint64_t initial_value) : value(initial_value) 
    {
    }

    // Checks if two order id's are exactly the same
    bool operator== (const CMEOrderId &rhs) const
    {
        return value == rhs.value;
    }
    
    // Checks if two order id's are different
    bool operator!= (const CMEOrderId &rhs) const
    {
        return (value != rhs.value);
    }
};

struct CMESymbol
{
    // Variables
    std::string value;

    // Functions
    // Constructor that initialises the struct's value member
    CMESymbol(std::string  initial_value) : value(initial_value) 
    {
    }

    // Checks if two symbols are exactly the same
    bool operator== (const CMESymbol &rhs) const
    {
        return value == rhs.value;
    }
    
    // Checks if two symbols are different
    bool operator!= (const CMESymbol &rhs) const
    {
        return (value != rhs.value);
    }
};
#endif //CME_CORE_TYPES_HPP