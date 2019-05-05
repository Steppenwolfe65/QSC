/**
* \file sysrand.h
* \brief <b>System random provider</b> \n
* Provides access to either the Windows CryptGenRandom provider or 
* the /dev/urandom pool on posix systems.
*
* \author John Underhill
* \date January 06, 2018
*/

#ifndef SYSRAND_H
#define SYSRAND_H

#include <cstdbool>
#include <stdint.h>

/* bogus integral type warnings */
/*lint -e970 */

/**
* \brief Get an array of pseudo-random bytes from the system entropy provider.
*
* \param buffer Pointer to the output byte array
* \param length The number of bytes to copy
* \return Returns true for success
*/
bool sysrand_getbytes(uint8_t* buffer, size_t length);

#endif
