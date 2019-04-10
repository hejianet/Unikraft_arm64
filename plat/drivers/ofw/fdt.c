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
#include <libfdt_env.h>
#include <fdt.h>
#include <libfdt.h>

#include <uk/print.h>

int fdt_getprop_u32_by_offset(const void *fdt, int offset,
		const char *name, uint32_t *out)
{
	const struct fdt_property *prop;
	int prop_len;

	prop = fdt_get_property_namelen(fdt, offset, name, strlen(name), &prop_len);
	if (!prop)
		return prop_len;

	if (prop_len >= (int)sizeof(fdt32_t)) {
		*out = fdt32_to_cpu(*(fdt32_t *)prop->data);
		return 0;
	}

	return -FDT_ERR_NOTFOUND;
}

static int fdt_find_irq_parent_offset(const void *fdt, int offset)
{
	uint32_t irq_parent;

	do {
		/* Find the interrupt-parent phandle */
		if (!fdt_getprop_u32_by_offset(fdt, offset,
				"interrupt-parent", &irq_parent))
			break;

		/* Try to find in parent node */
		offset = fdt_parent_offset(fdt, offset);
	} while (offset >= 0);

	if (offset < 0)
		return offset;

	/* Get interrupt parent node by phandle */
	return fdt_node_offset_by_phandle(fdt, irq_parent);
}

int fdt_interrupt_cells(const void *fdt, int offset)
{
	int intc_offset;
	int val;
	int ret;

	intc_offset = fdt_find_irq_parent_offset(fdt, offset);
	if (intc_offset < 0)
		return intc_offset;

	ret = fdt_getprop_u32_by_offset(fdt, intc_offset, "#interrupt-cells",
					(uint32_t *)&val);
	if (ret < 0)
		return ret;

	if ((val <= 0) || (val > FDT_MAX_NCELLS))
		return -FDT_ERR_BADNCELLS;

	return val;
}
