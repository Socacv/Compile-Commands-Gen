# Compile Commands Gen
## About
This is a very niche utility that i made for personal usage but wanted to share, it generates a compile commands.json for a c source file with the pkgconfig --cflags input. 
I made this since i use the clangd lsp and i don't really like to use cmake to generate the compile commands required by clangd for correct include dir handling.
