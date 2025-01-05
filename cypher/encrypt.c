#include "encrypt.h"
#include "mbedtls/aes.h"

#include <string.h>


contexte_cry* create_ctx_cry(){
    contexte_cry* ctx = malloc(sizeof(contexte_cry));
    return ctx;
}


int destroy_ctx_cry(contexte_cry* ctx_cry){
    free(ctx_cry);
    return 0;
}


int prepare_ctx_cry(contexte_cry* ctx_cry, unsigned char* key, unsigned int key_sz, unsigned char* iv, unsigned int iv_sz){
    ctx_cry->key = key;
    ctx_cry->key_sz = key_sz;
    ctx_cry->iv = iv;
    ctx_cry->iv_sz = iv_sz;
    
    return 0;
}



int encrypt_all_data(contexte_cry* ctx_cry, unsigned char* buffer_plain, unsigned int buffer_plain_sz, unsigned char* buffer_crypto, unsigned int* buffer_crypto_sz){

    unsigned char * key = ctx_cry->key;
    unsigned char * iv = ctx_cry->iv;
    unsigned int iv_sz = ctx_cry->iv_sz;

    unsigned char * padded_buffer_plain;
    unsigned char * iv_copy;

    int padding = 16 - (buffer_plain_sz % 16);
    padded_buffer_plain = (unsigned char*) malloc(*buffer_crypto_sz);

    if (padded_buffer_plain != NULL) {
            memcpy(padded_buffer_plain, buffer_plain, buffer_plain_sz);

            padded_buffer_plain[buffer_plain_sz] = 0x80;

            for (int i = buffer_plain_sz; i < padding; i++) {
                padded_buffer_plain[i] = 0;
            }
            
        }
        iv_copy = (unsigned char *) malloc(16);
        memcpy(iv_copy, iv, iv_sz);

        mbedtls_aes_context aes;
        mbedtls_aes_setkey_enc( &aes, key, 256 );
        mbedtls_aes_crypt_cbc( &aes, MBEDTLS_AES_ENCRYPT, *buffer_crypto_sz, iv, padded_buffer_plain, buffer_crypto );
        mbedtls_aes_free(&aes);

        ctx_cry->iv = iv_copy;
        
        return 0;
}


int decrypt_all_data(contexte_cry* ctx_cry, unsigned char* buffer_plain, unsigned int *buffer_plain_sz, unsigned char* buffer_crypto, unsigned int buffer_crypto_sz){

    mbedtls_aes_context aes_ctx;
    unsigned char *key = ctx_cry->key; 
    unsigned char *iv = ctx_cry->iv;
    
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_dec(&aes_ctx, key, 256);
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, buffer_crypto_sz, iv, buffer_crypto, buffer_plain);
    mbedtls_aes_free(&aes_ctx);

    *buffer_plain_sz = strlen(buffer_plain) -1;
    
    return 0;
}
