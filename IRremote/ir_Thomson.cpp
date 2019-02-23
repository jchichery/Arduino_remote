#include "IRremote.h"
#include "IRremoteInt.h"

//==============================================================================
//     TTTTTTTTT   H     H    OOO    MMM MMM    SSSS     OOO    N     N
//         T       H     H   O   O   M  M  M   S        O   O   N N   N
//         T       HHHHHHH   O   O   M  M  M    SSSS    O   O   N  N  N
//         T       H     H   O   O   M     M        S   O   O   N   N N
//         T       H     H    OOO    M     M    SSSS     OOO    N     N
//==============================================================================

#define THOMSON_BITS 24

#define THOMSON_HDR_MARK 3900
#define THOMSON_HDR_SPACE 3900
#define THOMSON_BIT_MARK 450
#define THOMSON_ONE_SPACE 2000
#define THOMSON_ZERO_SPACE 1000
#define THOMSON_RPT_LENGTH 60000

//+=============================================================================
#if SEND_THOMSON
void IRsend::sendTHOMSON(unsigned long data, int nbits)
{
    // Set IR carrier frequency
    enableIROut(38);

    // Header
    mark(THOMSON_HDR_MARK);
    space(THOMSON_HDR_SPACE);
    mark(THOMSON_BIT_MARK);

    // Data
    for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1)
    {
        if (data & mask)
        {
            space(THOMSON_ONE_SPACE);
            mark(THOMSON_BIT_MARK);
        }
        else
        {
            space(THOMSON_ZERO_SPACE);
            mark(THOMSON_BIT_MARK);
        }
    }
    space(0); // Always end with the LED off
}
#endif

//+=============================================================================
#if DECODE_THOMSON
bool IRrecv::decodeTHOMSON(decode_results *results)
{
    long data = 0;
    int offset = 1; // Skip first space

    // Check we have the right amount of data
    if (irparams.rawlen < (2 * THOMSON_BITS) + 4)
        return false;

    // Initial mark/space
    if (!MATCH_MARK(results->rawbuf[offset++], THOMSON_HDR_MARK))
        return false;
    if (!MATCH_SPACE(results->rawbuf[offset++], THOMSON_HDR_SPACE))
        return false;

    for (int i = 0; i < THOMSON_BITS; i++)
    {
        if (!MATCH_MARK(results->rawbuf[offset++], THOMSON_BIT_MARK))
            return false;

        if (MATCH_SPACE(results->rawbuf[offset], THOMSON_ONE_SPACE))
            data = (data << 1) | 1;
        else if (MATCH_SPACE(results->rawbuf[offset], THOMSON_ZERO_SPACE))
            data = (data << 1) | 0;
        else
            return false;
        offset++;
    }

    // Stop bit
    if (!MATCH_MARK(results->rawbuf[offset], THOMSON_BIT_MARK))
        return false;

    // Success
    results->bits = THOMSON_BITS;
    results->value = data;
    results->decode_type = THOMSON;
    return true;
}
#endif
