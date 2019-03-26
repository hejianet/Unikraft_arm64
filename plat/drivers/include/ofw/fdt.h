/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Wei Chen <Wei.Chen@arm.com>
 *          Jianyong Wu <Jianyong.Wu@arm.com>
 *
 * Copyright (c) 2018, Arm Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THIS HEADER MAY NOT BE EXTRACTED OR MODIFIED IN ANY WAY.
 */
#ifndef _PLAT_DRIVER_OFW_FDT_H
#define _PLAT_DRIVER_OFW_FDT_H

/**
 * fdt_getprop_u32_by_offset - retrieve u32 of a given property
 * @fdt: pointer to the device tree blob
 * @nodeoffset: offset of the node whose property to find
 * @name: name of the property to find
 * @out: pointer to u32 variable (will be overwritten) or NULL
 *
 * fdt_getprop_u32_by_offset() retrieves u32 to the value of the property
 * named 'name' of the node at offset nodeoffset (this will be a
 * pointer to within the device blob itself, not a copy of the value).
 * If out is non-NULL, the u32 of the property value is returned.
 *
 * returns:
 *	0, on success
 *		out contains the u32 of a given property at nodeoffset.
 *	-FDT_ERR_NOTFOUND, node does not have named property
 *	-FDT_ERR_BADNCELLS,
 */
int fdt_getprop_u32_by_offset(const void *fdt, int nodeoffset,
		const char *name, uint32_t *out);

/**
 * fdt_interrupt_cells - retrieve the number of cells needed to encode an
 *                       interrupt source
 * @fdt: pointer to the device tree blob
 * @nodeoffset: offset of the node to find the interrupt for.
 *
 * When the node has a valid #interrupt-cells property, returns its value.
 *
 * returns:
 *     0 <= n < FDT_MAX_NCELLS, on success
 *      -FDT_ERR_BADNCELLS, if the node has a badly formatted or invalid
 *             #interrupt-cells property
 *     -FDT_ERR_BADMAGIC,
 *     -FDT_ERR_BADVERSION,
 *     -FDT_ERR_BADSTATE,
 *     -FDT_ERR_BADSTRUCTURE,
 *     -FDT_ERR_TRUNCATED, standard meanings
 */
int fdt_interrupt_cells(const void *fdt, int nodeoffset);

/**
 * fdt_get_address - retrieve device address of a given index
 * @fdt: pointer to the device tree blob
 * @nodeoffset: offset of the node to find the address for.
 * @index: index of region
 * @addr: return the region address
 * @size: return the region size
 *
 * returns:
 *     0, on success
 *      -FDT_ERR_BADNCELLS, if the node has a badly formatted or invalid
 *             address property
 *      -FDT_ERR_NOTFOUND, if the node doesn't have address property
 *      -FDT_ERR_NOSPACE, if the node doesn't have address for index
 */
int fdt_get_address(const void *fdt, int nodeoffset, int index,
			uint64_t *addr, uint64_t *size);

/**
 * fdt_node_offset_by_compatible_list - find nodes with a given
 *                                     'compatible' list value
 * @fdt: pointer to the device tree blob
 * @startoffset: only find nodes after this offset
 * @compatibles: a list of 'compatible' string to match, should be ended
 * with NULL string.
 * fdt_node_offset_by_compatible_list() returns the offset of the
 * first matched node after startoffset, which has a 'compatible'
 * property which lists the given compatible string; or if
 * startoffset is -1, the very first such node in the tree.
 *
 * returns:
 *     structure block offset of the located node (>= 0, >startoffset),
 *              on success
 *     -FDT_ERR_NOTFOUND, no node matching the criterion exists in the
 *             tree after startoffset
 *     -FDT_ERR_BADOFFSET, nodeoffset does not refer to a BEGIN_NODE tag
 *     -FDT_ERR_BADMAGIC,
 *     -FDT_ERR_BADVERSION,
 *     -FDT_ERR_BADSTATE,
 *     -FDT_ERR_BADSTRUCTURE, standard meanings
 */
int fdt_node_offset_by_compatible_list(const void *fdt, int startoffset,
					const char * const compatibles[]);

#endif
