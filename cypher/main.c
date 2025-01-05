
#include "generation.h"
#include "key.h"
#include "encrypt.h"
#include "io.h"

#include <string.h>
#include <stdio.h>
#include <getopt.h>

#define READ 0x01
#define WRITE 0x02
#define CRYPTO 0x04
#define PLAIN 0x08


void print_usage(){
    printf("Usage : protect [option]\n");
    printf("    -c encryption mode \n");
    printf("    -d decryption mode\n");
    printf("    -p <password> password to use\n");
    printf("    -i <file> entry file\n");
    printf("    -o <file> output file\n");
    printf("    -h display this help\n");
}

int main(int argc, char *argv[]) {
    

    /*variables*/

    unsigned char iv[16];
    unsigned int iv_sz = 16;
    unsigned char key[32];
    unsigned int k_sz = 0;
    unsigned char* buffer_plain = NULL;
    unsigned int p_sz = 0;
    unsigned char* buffer_crypto = NULL;
    unsigned int c_sz = 0;
    contexte_io* io_crypto = NULL;
    contexte_io* io_plain = NULL;
    contexte_cry* cry = NULL;

    int opt= 0;
    int encryption = -1, decryption = -1;
    char *password = NULL, *input = NULL, *output = NULL;

    while ((opt = getopt(argc, argv,"hcdp:i:o:")) != -1) {
        switch (opt) {
            case 'h' : print_usage();
                exit(-1);
             case 'c' : encryption = 0;
                 break;
             case 'd' : decryption = 0;
                 break;
             case 'p' : password = optarg; 
                 break;
             case 'i' : input = optarg;
                 break;
             case 'o' : output = optarg;
                 break;
             default: print_usage(); 
                 exit(EXIT_FAILURE);
        }
    }
    if (password == NULL || input == NULL || output ==NULL) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    

    if(encryption == 0 && decryption == -1){
        

        io_plain = create_ctx_io();
        prepare_ctx_io(io_plain, input, READ|PLAIN);
        
        generate_iv(iv, iv_sz);
        
        build_key(password, strlen(password), key, &k_sz);
        
        cry = create_ctx_cry();
        prepare_ctx_cry(cry, key, k_sz, iv, iv_sz);
        
        io_crypto = create_ctx_io();
        prepare_ctx_io(io_crypto, output, WRITE|CRYPTO);

        p_sz = data_size(io_plain);
        buffer_plain = (unsigned char*) malloc(p_sz);

        c_sz = (p_sz / 16) * 16 + 16;
        buffer_crypto = (unsigned char*) malloc(c_sz);

        read_all_data(io_plain, buffer_plain, p_sz);

        encrypt_all_data(cry, buffer_plain, p_sz, buffer_crypto, &c_sz);
        
        ecrire_iv(io_crypto, cry->iv, cry->iv_sz);
        
        write_all_data(io_crypto, buffer_crypto, c_sz);

        destroy_ctx_cry(cry);
        destroy_ctx_io(io_plain);
        destroy_ctx_io(io_crypto);
        free(buffer_crypto);
        free(buffer_plain);
    
    } else if(encryption == -1 && decryption == 0){
        
        io_crypto = create_ctx_io();
        prepare_ctx_io(io_crypto, input, READ|CRYPTO);

        build_key(password, strlen(password), key, &k_sz);
        
        read_iv(io_crypto, iv, &iv_sz);
        
        cry = create_ctx_cry();
        prepare_ctx_cry(cry, key, k_sz, iv, iv_sz);
        
        io_plain = create_ctx_io();
        prepare_ctx_io(io_plain, output, WRITE|PLAIN);

        c_sz = data_size(io_crypto);
        buffer_crypto = (unsigned char*) malloc(c_sz);
        buffer_plain = (unsigned char*) malloc(c_sz);

        read_all_data(io_crypto, buffer_crypto, c_sz);

        decrypt_all_data(cry, buffer_plain, &p_sz, buffer_crypto, c_sz);

        write_all_data(io_plain, buffer_plain, p_sz);

        destroy_ctx_cry(cry);
        destroy_ctx_io(io_crypto);
        destroy_ctx_io(io_plain);
        free(buffer_crypto);
        free(buffer_plain);
        
    } else{
        print_usage();
    }
        
    return 0;
}
