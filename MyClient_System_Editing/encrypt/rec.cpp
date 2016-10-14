void Save_Item_Proto::Compress(const unsigned char *decompressedBuffer, int decompressedSize)
{
    lzo_init();
    lzo_voidp wrkmem = (lzo_voidp)malloc(LZO1X_999_MEM_COMPRESS);
    LPBYTE compressedBuffer = (LPBYTE)calloc(COMP_BUF_CAL(decompressedSize), 1);

    lzo_uint compressedSize = 0;
    lzo1x_999_compress(decompressedBuffer, decompressedSize, compressedBuffer, &compressedSize, wrkmem);

    LPBYTE compressedBufferMCOZ = (LPBYTE)calloc(compressedSize + 4, 1);
    memcpy(compressedBufferMCOZ, (char *)"MCOZ", 4);
    memcpy(compressedBufferMCOZ + 4, compressedBuffer, compressedSize);

    this->data = new unsigned char[compressedSize + 4];
    memset(this->data, 0, compressedSize + 4);

    this->compSize = compressedSize;
    this->encryptSize = compressedSize + 4;
    this->decompSize = decompressedSize;
    this->itemCount = decompressedSize / 152;

    m2crypt::encrypt(this->data, compressedBufferMCOZ, this->xteaKey, this->encryptSize);

    LPBYTE finalBuffer = (LPBYTE)calloc(this->encryptSize + 36, 1);
    int version = 1, stride = 152;
    memcpy(finalBuffer, (char *)"MIPX", 4);
    memcpy(finalBuffer + 4, &version, 4);
    memcpy(finalBuffer + 8, &stride, 4);
    memcpy(finalBuffer + 12, &this->itemCount, 4);
    memcpy(finalBuffer + 16, &this->decompSize, 4);
    memcpy(finalBuffer + 20, (char *)"MCOZ", 4);
    memcpy(finalBuffer + 24, &this->encryptSize, 4);
    memcpy(finalBuffer + 28, &this->compSize, 4);
    memcpy(finalBuffer + 32, &this->decompSize, 4);
    memcpy(finalBuffer + 36, this->data, this->encryptSize);

    FILE *fIP;
    fIP = fopen("item_proto", "wb");
    fwrite(finalBuffer, 1, this->encryptSize + 36, fIP);
    fclose(fIP);
}