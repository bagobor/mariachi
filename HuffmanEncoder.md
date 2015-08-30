# Introduction #

This is the typical usage of the huffman encoder

# Details #

```
// allocates the huffman object
Huffman huffamn;

// encodes the given file
huffamn.encode("file.ext", "file.ext.encoded");

// decodes the given file
huffamn.decode("file.ext.encoded", "file.ext.decoded");
```