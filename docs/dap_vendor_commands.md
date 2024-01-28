DAPLink CMSIS-DAP vendor commands
=================================

## DAP_GetInterfaceCapabilities

This command returns a vector of capability bits that indicate the presence or absence of features in DAPLink's interface firmware.

**Command ID**: 0x85 (`ID_DAP_Vendor5`)

**Request**

```
  BYTE |
> 0x85 |
 ******|
 ```

 **Response**

 ```
  BYTE | BYTE | WORD |
< 0x85 | Len  | Caps |
 ******|******|++++++|
```

After the command ID, the response contains () a single `Len` byte with the number of capability words. Following is an array of `Len` 32-bit words encoded as little endian.

**Capability bit definitions**

- [0] `kCapability_FileFormatBinary`: Programming of binary firmware files is supported.
- [1] `kCapability_FileFormatIntelHex`: Programming of Intel hex files is supported.
