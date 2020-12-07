/* The GPL version 3 License (GPLv3)
*
* Copyright (c) 2020 Digital Freedom Defence Inc.
* This file is part of the QSC Cryptographic library
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*
* Implementation Details:
* A FIFO general purpose data queue.
* Written by John G. Underhill
* Updated on November 11, 2020
* Contact: develop@vtdev.com */

#ifndef QSC_QUEUE_H
#define QSC_QUEUE_H

#include "common.h"
#include "intutils.h"
#include "memutils.h"

/*!
\def QSC_QUEUE_ALIGNMENT
* The internal memory alignment constant
*/
#define QSC_QUEUE_ALIGNMENT 64

/*!
\def QSC_QUEUE_MAX_DEPTH
* The maximum queue depth
*/
#define QSC_QUEUE_MAX_DEPTH 64

/*! \struct qsc_queue_state
* Contains the queue context state
*/
typedef struct qsc_queue_state
{
	uint8_t** queue;					/*!< The pointer to a 2 dimensional queue array */
	uint64_t tags[QSC_QUEUE_MAX_DEPTH];	/*!< The 64-bit tag associated with each queue item  */
	size_t count;						/*!< The number of queue items */
	size_t depth;						/*!< The maximum number of items in the queue */
	size_t position;					/*!< The next empty slot in the queue */
	size_t width;						/*!< The maximum byte length of a queue item */

} qsc_queue_state;


/**
* \brief Destroy the queue state.
*
* \param state [struct] The function state
*/
QSC_EXPORT_API void qsc_queue_destroy(qsc_queue_state* ctx);

/**
* \brief Flush the content of the queue to an array.
*
* \param state [struct] The function state
* \param output [array] The array receiving the queue items
*/
QSC_EXPORT_API void qsc_queue_flush(qsc_queue_state* ctx, uint8_t* output);

/**
* \brief Initialize the queue state.
*
* \param state [struct] The function state
* \param depth [size] The number of queue items to initialize, maximum is QSC_QUEUE_MAX_DEPTH
* \param width [size] The maximum size of each queue item in bytes
*/
QSC_EXPORT_API void qsc_queue_initialize(qsc_queue_state* ctx, size_t depth, size_t width);

/**
* \brief Get the number of items in the queue.
*
* \param state [struct] The function state
* \return The number of items in the queue
*/
QSC_EXPORT_API size_t qsc_queue_items(qsc_queue_state* ctx);

/**
* \brief Get the full status from the queue.
*
* \param state [struct] The function state
* \return Returns true if the queue is full
*/
QSC_EXPORT_API bool qsc_queue_isfull(qsc_queue_state* ctx);

/**
* \brief Get the empty status from the queue.
*
* \param state [struct] The function state
* \return Returns true if the queue is empty
*/
QSC_EXPORT_API bool qsc_queue_isempty(qsc_queue_state* ctx);

/**
* \brief Returns the first member of the queue, and erases that item from the queue.
*
* \param state [struct] The function state
* \param output [array] The array receiving the queue item
* \param outlen [size] The number of bytes to copy from the queue item
* \return The items associated tag
*/
QSC_EXPORT_API uint64_t qsc_queue_pop(qsc_queue_state* ctx, uint8_t* output, size_t outlen);

/**
* \brief Add an item to the queue.
*
* \param state [struct] The function state
* \param input [array] The array item to be added to thr queue
* \param inlen [size] The byte size of the the queue item to be added
* \param tag [integer] The items associated tag
*/
QSC_EXPORT_API void qsc_queue_push(qsc_queue_state* ctx, const uint8_t* input, size_t inlen, uint64_t tag);

/**
* \brief The queing functions self test.
*
* \return [bool] Returns true upon success
*/
QSC_EXPORT_API bool qsc_queue_self_test();

#endif
