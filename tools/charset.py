#!/usr/bin/env python
# -*- coding: utf-8 -*-

# used ? Δ~\n\t

import os

standard_chars = """ !"#$%&'()*+,-./0123456789:;<=>?°ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz~|≥"""

extra_chars = ""


special_chars = {
    "en": ""
}

# print("CN charset: %d symbols" % len(special_chars["cn"]))


def get_chars(subset):
    result = standard_chars + extra_chars
    if subset == "all":
        for key, chars in special_chars.items():
            result += "".join([char for char in chars if char not in result])
    else:
        if subset in special_chars:
            result += "".join([char for char in special_chars[subset] if char not in subset_lowercase])
    return result


def get_chars_encoding(subset):
    result = {}
    if subset == "all":
        chars = get_chars(subset)
        for char in chars:
            if char in special_chars["cn"]:
                index = special_chars["cn"].index(char) + 1
                if index >= 0x100:
                    index += 1
                result[char] = "\\%03o\\%03o" % (0xFE + ((index >> 8) & 0x01), index & 0xFF)
            elif char not in standard_chars and char not in extra_chars:
                result[char] = "\\%03o" % (0xC0 + chars.index(char) - len(standard_chars))
    else:
        offset = 128 - len(standard_chars)
        chars = get_chars(subset)
        for char in chars:
            if char not in standard_chars:
                result[char] = "\\%03o" % (offset + chars.index(char))
    return result
