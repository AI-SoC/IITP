
/* Teshbench Generator by Seunghyun Park. Version 2.1
 * ram startup fixed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage( char *argv[]);

char *SubStr( int s, int e, char *str );

int FindStrInStr( char *StrIn, char *StrRef, int Bias);

int main( int argc, char *argv[])
{
    FILE *fp_sti, *fp_sti_gen, *fp_src, *fp_asm, *fp_tb, *fp_tb_origin, *fp_tb_gen;
    int i, j;

    int log = 1;
    char event_driven_type_2 = 0;
    
    char str[512];
    char str_sti[256];
    char str_tb[256];
    char str_tb_tmp[256];
    char* str_tb_origin;
    char str_sti_tmp[256];
    char str_sti_gen[256];
    char str_tb_gen[256];
    char **str_src;
    char **str_asm;
    int  n_src;
    int  n_line;
    char str_dummy[512];
    char str_dummy_temp[512];
    char str_address[64];

    char str_seg[256];

    int  src_ok = 1, sti_ok = 1, tb_ok = 1;

    int  find_address;

    int  first;


    str_src = new char * [256];
    str_asm = new char * [256];
    for( i=0; i<256; i++)
    {
        str_src[i] = new char [256];
        str_asm[i] = new char [256];
    }
    n_src = 0;

    // analysis option
    for( i=1; i<argc; )
        if( !strcmp(argv[i],"-o") )
        {
            sti_ok = 0;
            strcpy( str_sti, argv[i+1] );
            sprintf( str_sti_tmp, "__tmp__sti__" );
            sprintf( str_sti_gen, "__gen__sti__" );
            i += 2;
        }
        else if( !strcmp(argv[i],"-t") )
        {
            tb_ok = 0;
            strcpy( str_tb, argv[i+1] );
            strcpy( str_tb_tmp, str_tb );
            str_tb_origin = strtok( str_tb_tmp, ".v" );
            sprintf( str_tb_origin, "%s_ORIGIN.v", str_tb_tmp );
            sprintf( str_tb_gen, "__gen__tb__" );
            i += 2;
        }
        else if( !strcmp(argv[i],"-s") )
        {
            log = 0;
            i += 1;
        }
        else if( !strcmp(argv[i],"-c") )
        {
            strcpy( str_src[n_src], argv[i+1]);
            strcpy( str_asm[n_src], argv[i+2]);
            src_ok = 0;
            n_src ++;
            i += 3;
            if( n_src == 256 )
            {
                fprintf( stderr, "Error: over the max c source file 256\n");
                exit(1);
            }
        }
        else
        {
            usage(argv);
            exit(1);
        }

    if( src_ok || sti_ok || tb_ok )
    {
        usage(argv);
        exit(1);
    }

    if( log )
    {
        printf("output file  : %s\n", str_sti);
        printf("# of c source file: %d\n", n_src);
    }

    fp_sti = fopen( str_sti, "w");
    if( fp_sti == NULL )
    {
        fprintf( stderr, "\nError: file open error: %s\n", str_sti);
        exit(1);
    }

    if( log ) printf("analize c source file\n");
    for( i=0; i<n_src; i++ )
    {
        first = 1;
        fp_src = fopen( str_src[i], "r");
        if( fp_src == NULL )
        {
            fprintf( stderr, "\nError: file open error: %s\n", str_src[i]);
            exit(1);
        }

        if( log )
        {
            printf(" + c source file: %s,", str_src[i]);
            printf(" asm file: %s\n", str_asm[i]);
            printf(" | + find the non event driven stimulus\n");
        }
        fprintf( fp_sti, "\n");
        fprintf( fp_sti, "//******************************\n");
        fprintf( fp_sti, "// Stimulus Generator\n");
        fprintf( fp_sti, "// c source file: %s\n", str_src[i]);
        fprintf( fp_sti, "//      asm file: %s\n", str_asm[i]);
        fprintf( fp_sti, "//******************************\n");
        fprintf( fp_sti, "\n");
        fprintf( fp_sti, "// non event driven stimulus\n");
        if( log ) printf(" | | + following stimulus is added !!\n");
        while( fgets( str, 512, fp_src) != NULL )
        {
            if( FindStrInStr( str, "//>", 0) != 0 )
            {
                if( log ) printf(" | | |   ");
                for( j=3; j<strlen(str); j++)
                {
                    fprintf( fp_sti, "%c", str[j]);
                    if( log ) printf("%c", str[j]);
                }
            }
            else if( FindStrInStr( str, "/*>", 0) != 0 )
            {
                if( log ) printf(" | | |   ");
                for( j=3; j<strlen(str); j++)
                {
                    fprintf( fp_sti, "%c", str[j]);
                    if( log ) printf("%c", str[j]);
                }
                while( fgets( str, 512, fp_src) != NULL )
                {
                    if( FindStrInStr( str, ">*/", 0) != 0 )
                        break;
                    for( j=0; j<strlen(str); j++)
                    {
                        if( j > 3 | str[j] != '/' | str[j] != '>' )
                        {
                            fprintf( fp_sti, "%c", str[j]);
                            if( log ) printf("%c", str[j]);
                        }
                    }
                }
            }
        }
        fprintf( fp_sti, "\n");
        n_line = 0;

        rewind( fp_src );
        fprintf( fp_sti, "// event driven stimulus\n\n");
        if( log ) printf(" | + find the event driven stimulus\n");

        // copy the asm file initially
        fp_asm     = fopen( str_asm[i], "r");
        fp_sti_gen = fopen( str_sti_gen, "w");
        while( fgets( str, 512, fp_asm) != NULL )
            fprintf( fp_sti_gen, "%s", str);
        fclose(fp_asm);
        fclose(fp_sti_gen);

        while( fgets( str, 512, fp_src) != NULL )
        {
            n_line ++;

            if( FindStrInStr( str, "//{", 0) != 0 || FindStrInStr( str, "/*{", 0) != 0 )
            {
                if( FindStrInStr( str, "/*{", 0) != 0  )
                    event_driven_type_2 = 1;
                else
                    event_driven_type_2 = 0;

                if( first )
                    fp_asm = fopen( str_asm[i], "r");
                else
                    fp_asm = fopen( str_sti_tmp, "r");

                fp_sti_gen = fopen( str_sti_gen, "w");
                first = 0;
                sprintf( str_dummy_temp, "%s:%d", str_src[i], n_line-1);
                sprintf( str_dummy, "%s", SubStr(2, strlen(str_dummy_temp)-1, str_dummy_temp) );
                find_address = 0;

                while( fgets( str, 512, fp_asm) != NULL )
                {
                    fprintf( fp_sti_gen, "%s", str);
                    if( FindStrInStr( str, str_dummy, 0) != 0 )
                    {
                        find_address = 1;
                        while( fgets( str, 512, fp_asm) != NULL )
                        {
                            fprintf( fp_sti_gen, "%s", str);
                            // when there exist code, that is the real address
                            if( FindStrInStr( str, " 800", 0 ) != 0 )
                            {
                                sscanf( str, "%7s", str_address);
                                if( log ) printf(" | | + following stimulus is added !!\n");
                                fprintf( fp_sti_gen, "sti always @ ( `A )\n");
                                if( log ) printf(" | | |   ");
                                if( log ) printf("always @ ( `A )\n");
                                fprintf( fp_sti_gen, "sti if( `A === 32'h%s )\n", str_address);
                                if( log ) printf(" | | |   ");
                                if( log ) printf("if( `A === 32'h%s )\n", str_address);
                                fprintf( fp_sti_gen, "sti begin\n");
                                if( log ) printf(" | | |   ");
                                if( log ) printf("begin\n");
                                while( fgets( str, 512, fp_src) != NULL )
                                {
                                    n_line ++;

                                    if( event_driven_type_2 == 1 )
                                    {
                                        if( FindStrInStr( str, "}*/", 0) == 0 )
                                        {
                                            if( log ) printf(" | | |   ");
                                            fprintf( fp_sti_gen, "sti ");
                                            for( j=0; j<strlen(str); j++)
                                            {
                                                fprintf( fp_sti_gen, "%c", str[j]);
                                                if( log ) printf("%c", str[j]);
                                            }
                                        }
                                        else
                                            break;
                                    }
                                    else
                                    {
                                        if( FindStrInStr( str, "//}", 0) == 0 )
                                        {
                                            // remove "//"
                                            for( j=0; ; j++)
                                                if( str[j] == '/' )
                                                    break;
                                            j += 2;
                                            if( log ) printf(" | | |   ");
                                            fprintf( fp_sti_gen, "sti ");
                                            for( ; j<strlen(str); j++)
                                            {
                                                fprintf( fp_sti_gen, "%c", str[j]);
                                                if( log ) printf("%c", str[j]);
                                            }
                                        }
                                        else
                                            break;
                                    }
                                }
                                fprintf( fp_sti_gen, "sti end\n\n");
                                if( log ) printf(" | | |   ");
                                if( log ) printf("end\n");
                                while( fgets( str, 512, fp_asm) != NULL )
                                    fprintf( fp_sti_gen, "%s", str);
                                fclose(fp_sti_gen);
                                fclose(fp_asm);
                                fp_sti_gen = fopen( str_sti_gen, "r");
                                fp_asm     = fopen( str_sti_tmp, "w");
                                while( fgets( str, 512, fp_sti_gen) != NULL )
                                    fprintf( fp_asm, "%s", str);
                                fclose(fp_asm);
                                fclose(fp_sti_gen);
                                break;
                            }
                            else if( FindStrInStr( str, "2000", 0 ) != 0 )
                            {
                                sscanf( str, "%8s", str_address);
                                if( log ) printf(" | | + following stimulus is added !!\n");
                                fprintf( fp_sti_gen, "sti always @ ( `A )\n");
                                if( log ) printf(" | | |   ");
                                if( log ) printf("always @ ( `A )\n");
                                fprintf( fp_sti_gen, "sti if( `A === 32'h%s )\n", str_address);
                                if( log ) printf(" | | |   ");
                                if( log ) printf("if( `A === 32'h%s )\n", str_address);
                                fprintf( fp_sti_gen, "sti begin\n");
                                if( log ) printf(" | | |   ");
                                if( log ) printf("begin\n");
                                while( fgets( str, 512, fp_src) != NULL )
                                {
                                    n_line ++;

                                    if( event_driven_type_2 == 1 )
                                    {
                                        if( FindStrInStr( str, "}*/", 0) == 0 )
                                        {
                                            if( log ) printf(" | | |   ");
                                            fprintf( fp_sti_gen, "sti ");
                                            for( j=0; j<strlen(str); j++)
                                            {
                                                fprintf( fp_sti_gen, "%c", str[j]);
                                                if( log ) printf("%c", str[j]);
                                            }
                                        }
                                        else
                                            break;
                                    }
                                    else
                                    {
                                        if( FindStrInStr( str, "//}", 0) == 0 )
                                        {
                                            // remove "//"
                                            for( j=0; ; j++)
                                                if( str[j] == '/' )
                                                    break;
                                            j += 2;
                                            if( log ) printf(" | | |   ");
                                            fprintf( fp_sti_gen, "sti ");
                                            for( ; j<strlen(str); j++)
                                            {
                                                fprintf( fp_sti_gen, "%c", str[j]);
                                                if( log ) printf("%c", str[j]);
                                            }
                                        }
                                        else
                                            break;
                                    }
                                }
                                fprintf( fp_sti_gen, "sti end\n\n");
                                if( log ) printf(" | | |   ");
                                if( log ) printf("end\n");
                                while( fgets( str, 512, fp_asm) != NULL )
                                    fprintf( fp_sti_gen, "%s", str);
                                fclose(fp_sti_gen);
                                fclose(fp_asm);
                                fp_sti_gen = fopen( str_sti_gen, "r");
                                fp_asm     = fopen( str_sti_tmp, "w");
                                while( fgets( str, 512, fp_sti_gen) != NULL )
                                    fprintf( fp_asm, "%s", str);
                                fclose(fp_asm);
                                fclose(fp_sti_gen);
                                break;
                            }
                        }
                        break;
                    }
                }

                // stimulus exist in the C source file
                // but not in the asm file
                // some may lost due to the macro definition
                if( find_address == 0 )
                {
                    fclose(fp_sti_gen);
                    fclose(fp_asm);
                    fp_sti_gen = fopen( str_sti_gen, "r");
                    fp_asm     = fopen( str_sti_tmp, "w");
                    while( fgets( str, 512, fp_sti_gen) != NULL )
                        fprintf( fp_asm, "%s", str );
                    fclose(fp_asm);
                    fclose(fp_sti_gen);
                }
            }
        }

        // dump the stimulus & pattern
        fp_sti_gen = fopen( str_sti_gen, "r" );
        while( fgets( str, 512, fp_sti_gen ) != NULL )
        {
            if( str[0] == 's' && str[1] == 't' && str[2] == 'i' )
                for( j=4; j<strlen(str); j++)
                    if( str[j] == NULL )
                        break;
                    else
                        fprintf( fp_sti, "%c", str[j]);
        }
        fclose(fp_sti_gen);
        fclose(fp_src);
    }

    fclose(fp_sti);

    // copy testbench first except for endmodule
    fp_tb_origin = fopen( str_tb_origin, "r");
    fp_tb_gen = fopen( str_tb_gen, "w" );
    while( fgets( str, 512, fp_tb_origin ) != NULL )
        if( FindStrInStr( str, "endmodule", 0 ) != 0 )
            break;
        else
            fprintf( fp_tb_gen, "%s", str );
    fclose(fp_tb_origin);
    fclose(fp_tb_gen);

    // attach to testbench
    fp_tb = fopen( str_tb, "w");
    fp_tb_gen = fopen( str_tb_gen, "r");
    fp_sti = fopen( str_sti, "r");
    while( fgets( str, 512, fp_tb_gen ) != NULL )
        fprintf( fp_tb, "%s", str );
    while( fgets( str, 512, fp_sti ) != NULL )
        fprintf( fp_tb, "%s", str );
    fprintf( fp_tb, "\n\nendmodule" );

    fclose(fp_tb);
    fclose(fp_tb_gen);
    fclose(fp_sti);

    // free source pointer
    for( i=0; i<256; i++)
    {
        delete str_src[i];
        delete str_asm[i];
    }
    delete str_src;
    delete str_asm;
}

char *SubStr( int s, int e, char *str )
{
    char *NEW = (char *)malloc(sizeof(char) * (e-s+2));
    strncpy(NEW, str+s, e-s+1);
    NEW[e-s+1] = 0;
    return NEW;
}

int FindStrInStr( char *StrIn, char *StrRef, int Bias)
{
    int InLength, RefLength;
    int i, j, Match = 0;
    InLength = strlen(StrIn);
    RefLength = strlen(StrRef);

    for( i=Bias, j=0; i<InLength; i++)
    {
    	if( StrIn[i] == StrRef[j] )
    	{
    		j++;
    		if( j == RefLength )
    			return i+1;
    	}
    	else if( j != 0 )
    		j = 0;
    }

    return 0;
}

void usage( char *argv[])
{
    fprintf( stderr, "usage:\n");
    fprintf( stderr, "\t%s [option]\n", argv[0]);
    fprintf( stderr, "\t\t-s : silent mode. print no message.\n");
    fprintf( stderr, "\t\t-o <stimuls output file>\n");
    fprintf( stderr, "\t\t-t <testbench file>\n");
    fprintf( stderr, "\t\t-c <c source file 0> <asm file 0>\n");
    fprintf( stderr, "\t\t-c <c source file 1> <asm file 1>\n");
    fprintf( stderr, "\t\t-c <c source file 2> <asm file 2>\n");
    fprintf( stderr, "\t\t...\n");
    fprintf( stderr, "\texample) %s -o ../STI/cpu.sti -t tb_ARM_ALU.v -c SRC/cpu.c OUT/cpu.S ... \n", argv[0]);
}
