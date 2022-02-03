/* 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#include "hid_rip.h"
#include "hid.h"

void hidrip_init_state(tuh_hid_rip_state_t *state) {
  state->stack_index = 0;
  state->usage_count = 0;
  memset(&state->global_items, 0, sizeof(uint8_t*) * HID_REPORT_STACK_SIZE * 16);
  memset(&state->local_items, 0, sizeof(uint8_t*) * 16);
  memset(&state->usages, 0, sizeof(uint32_t) * HID_REPORT_MAX_USAGES);
}

int16_t hidrip_parse_item(tuh_hid_rip_state_t *state, uint8_t *ri, uint16_t length) {
  int16_t il = hidri_size(ri, length);
  if (il > 0) {
    if (hidri_is_long(ri)) {
      // For now just don't do anything with long items.
    }
    else {
      uint8_t short_type = hidri_short_type(ri);
      uint8_t short_tag = hidri_short_tag(ri);
      switch (short_type) {
        case RI_TYPE_GLOBAL:
          state->global_items[state->stack_index][short_tag] = ri;
          switch (short_tag) {
            case RI_GLOBAL_PUSH:
              if (++state->stack_index == HID_REPORT_STACK_SIZE) return -1; // TODO enum? Stack overflow
              memcpy(&state->global_items[state->stack_index], &state->global_items[state->stack_index - 1], sizeof(uint8_t*) * 16);
              break;
            case RI_GLOBAL_POP:
              if (state->stack_index-- == 0) return -2; // TODO enum? Stack underflow
              break;
            default:
              break;
          }
          break;
        case RI_TYPE_LOCAL:
          switch(short_tag) {
            case RI_LOCAL_USAGE: {
              uint32_t usage = hidri_short_udata32(ri);
              if (hidri_short_data_length(ri) <= 2) {
                uint8_t* usage_page_item = state->global_items[state->stack_index][RI_GLOBAL_USAGE_PAGE];
                uint32_t usage_page = usage_page_item ? hidri_short_udata32(usage_page_item) : 0;
                usage |= usage_page << 16;
              }
              if (state->usage_count == HID_REPORT_MAX_USAGES) return -3; // TODO enum? Max usages overflow
              state->usages[state->usage_count++] = usage;
              break;
            }
            default:
              state->local_items[short_tag] = ri;
              break;
          }
          break;
        case RI_TYPE_MAIN:
          // Hmmm ??!?
          // TODO handle the main item
          // ...then clear down local state... maybe not here!
          memset(&state->local_items, 0, sizeof(uint8_t*) * 16);
          state->usage_count = 0;
          break;
          
        default:
          break;
      }
    }
  }
  return il;
}


