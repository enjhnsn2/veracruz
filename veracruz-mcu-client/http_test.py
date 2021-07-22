#!/usr/bin/env python3
#
# ## Authors
#
# The Veracruz Development Team.
#
# ## Licensing and copyright notice
#
# See the `LICENSE.markdown` file in the Veracruz root directory for
# information on licensing and copyright.
#

import http.client
import base64

data = [
    0x0a, 0x20, 0xbf, 0x5d, 0xd2, 0xb2, 0xc6, 0x28, 0x6d, 0xee, 0xe2, 0xf2, 0x07, 0x2a, 0xfb, 0xbc,
    0xa2, 0x65, 0x15, 0x87, 0x3d, 0xa1, 0x2e, 0x51, 0xd8, 0xdc, 0x48, 0x1b, 0x73, 0x18, 0x61, 0xe5,
    0x0d, 0x54                                          
]

http.client.HTTPConnection.debuglevel = 1

c = http.client.HTTPConnection('172.17.0.2', 3017, timeout=3)
c.request("POST", "/sinaloa", body=base64.b64encode(b''.join(c.to_bytes(1, 'little') for c in data)))
print(repr(c.getresponse()))

#c = http.client.HTTPConnection('172.17.0.2', 3017, timeout=3)
#c.request("GET", "/")
#print(c.getresponse())
