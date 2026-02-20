/**
 * ============================================================================
 * EnumFactory Usage Example: Stock Market Data Mapping
 * ============================================================================
 * 
 * DESCRIPTION:
 * This file demonstrates how to use the EnumFactory library to generate
 * advanced Enumerations with associated "Multi-Attribute" properties.
 * 
 * Using a single source of truth (the X-Macro list `STOCK_ENUM`), we 
 * instruct the library to perform the following:
 * 
 * 1. Generate an Enum called `STOCK` with assigned values.
 * 2. Generate a count threshold constant (`STOCK_total`) and an item count (`STOCK_count`).
 * 3. Generate parallel data maps representing:
 *      a. The stock's exact ID number.
 *      b. The stock's price (float).
 *      c. The stock's market sector (const char*).
 *      d. The stock's market capitalization in billions (int).
 * 4. Ensure memory safety using the compile-time bounds-checker `ENUM_IS_VALID()`.
 * 
 * ============================================================================
 */

#include <stdio.h>
#include "../enumfactory/include/enumfactorymacros.h"

/**
 * ----------------------------------------------------------------------------
 * 1. Define the Single Source of Truth
 * ----------------------------------------------------------------------------
 * By applying the X-Macro pattern, we encapsulate all enum members and their
 * relational attributes into a single generator macro.
 * 
 * Data Schema (Tuple format passed to X):
 * X(G, ENUM_MEMBER, ID_VALUE, PRICE, SECTOR_NAME, MARKET_CAP_BILLIONS)
 */
#define STOCK_ENUM(X, G) \
    X(G, AAPL,  1,  180.50f, "Technology", 2800) \
    X(G, MSFT,  2,  415.50f, "Technology", 3100) \
    X(G, GOOGL, 3,  145.20f, "Communication", 1800) \
    X(G, AMZN,  4,  175.35f, "Consumer Cyclical", 1850) \
    X(G, NVDA,  5,  850.10f, "Technology", 2100) \
    X(G, META,  6,  480.00f, "Communication", 1200) \
    X(G, TSLA,  7,  190.50f, "Consumer Cyclical", 600) \
    X(G, BRK_B, 8,  410.20f, "Financials", 880) \
    X(G, LLY,   9,  750.00f, "Healthcare", 710) \
    X(G, TSM,   10, 140.00f, "Technology", 720)


/**
 * ----------------------------------------------------------------------------
 * 2. Core Enum & Initialization
 * ----------------------------------------------------------------------------
 * Since our X-Macro generator `STOCK_ENUM` supplies explicitly assigned ID 
 * values (1, 2, 3...) as the first parameter after the identifier, we use 
 * `ENUM_VALUE_ASSIGN` to bind these IDs directly to the enum members.
 * 
 * We also simultaneously map the ID Value array (index 0) using ENUMS_MAP.
 * 
 * Generates: `enum STOCK` and `int STOCK_get_id(int value)`
 */
ENUMS_MAP(STOCK, STOCK_ENUM, ENUM_VALUE_ASSIGN, int, id);

/*
 * Generated Enum: STOCK
 * -------------------------
 * Type: Map (Enum and Multi-Attribute Arrays)
 * Actual Member Count: 10
 * Range (total): 0 to 11 (exclusive)
 *
 * Mappings:
 * - AAPL  = 1  (Price: 180.50,  Sector: "Technology",        Market Cap: 2800)
 * - MSFT  = 2  (Price: 415.50,  Sector: "Technology",        Market Cap: 3100)
 * - GOOGL = 3  (Price: 145.20,  Sector: "Communication",     Market Cap: 1800)
 * - AMZN  = 4  (Price: 175.35,  Sector: "Consumer Cyclical", Market Cap: 1850)
 * - NVDA  = 5  (Price: 850.10,  Sector: "Technology",        Market Cap: 2100)
 * - META  = 6  (Price: 480.00,  Sector: "Communication",     Market Cap: 1200)
 * - TSLA  = 7  (Price: 190.50,  Sector: "Consumer Cyclical", Market Cap: 600)
 * - BRK_B = 8  (Price: 410.20,  Sector: "Financials",        Market Cap: 880)
 * - LLY   = 9  (Price: 750.00,  Sector: "Healthcare",        Market Cap: 710)
 * - TSM   = 10 (Price: 140.00,  Sector: "Technology",        Market Cap: 720)
 */


/**
 * ----------------------------------------------------------------------------
 * 3. Generating Parallel Attribute Arrays
 * ----------------------------------------------------------------------------
 * Here we extract secondary, tertiary, and downstream columns out of the 
 * macro tuple sequence without creating runtime hash maps or struct arrays.
 * 
 * It parses exactly the target subset of data dynamically via variadic indices.
 */

// Generates float STOCK_get_price(int value). Extracts Index 1 from tuple.
ENUMS_ARRAY(STOCK, STOCK_ENUM, float, price, 1);

// Generates const char* STOCK_get_sector(int value). Extracts Index 2.
ENUMS_ARRAY(STOCK, STOCK_ENUM, const char*, sector, 2);

// Generates int STOCK_get_mcap(int value). Extracts Index 3.
ENUMS_ARRAY(STOCK, STOCK_ENUM, int, mcap, 3);


/**
 * ----------------------------------------------------------------------------
 * 4. Helper API Endpoints
 * ----------------------------------------------------------------------------
 * Registers `const char* STOCK_to_string(int value)` to provide native C-string
 * stringification without writing `switch/case` sequences manually.
 */
ENUM_TO_STRING(STOCK);


/**
 * ----------------------------------------------------------------------------
 * Execution & Validation
 * ----------------------------------------------------------------------------
 */
int main() {
    printf("\n=== Top 10 Stock Market Report ===\n");
    printf("-----------------------------------------------------------------\n");
    printf("%-10s | %-5s | %-8s | %-18s | %-10s\n", "Enum ID", "Val", "Price", "Sector", "Market Cap");
    printf("-----------------------------------------------------------------\n");

    /* 
     * Iteration Mechanics:
     * We loop up to `STOCK_total`, which statically compiles as the highest 
     * possible assigned integer in the list + 1. (In this case, 11).
     */
    for (int i = 0; i < STOCK_total; i++) {
        
        /* 
         * Robust Type Safety bounds-checking!
         * Integer '0' does not strictly match any enum value in our manually 
         * assigned sequence (they start at 1). `ENUM_IS_VALID` intelligently 
         * filters out gaps avoiding corrupted memory or undefined operations.
         */
        if (!ENUM_IS_VALID(STOCK, i)) {
            continue;
        }

        /* 
         * Pulling directly from the generated static O(1) jump tables. 
         * The C compiler reduces all these calls directly to efficient maps.
         */
        printf("%-10s | %-5d | $%-7.2f | %-18s | $%d B\n",
            STOCK_to_string(i),
            STOCK_get_id(i),
            STOCK_get_price(i),
            STOCK_get_sector(i),
            STOCK_get_mcap(i)
        );
    }

    printf("-----------------------------------------------------------------\n");
    printf("Total valid stocks defined: %d\n", STOCK_count); // Always equates perfectly to 10.

    return 0;
}
