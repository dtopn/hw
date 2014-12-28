#include <stdio.h>
main()
{
    int ret;
    FILE *fp_read;
    FILE *fp_write;
    unsigned char buf[2048];

    fp_read=fopen("./yzzfs","rb");

    if(fp_read == NULL)
    {
        printf("open yzzfs failed!\n");
        return 1;
    }

    fp_write=fopen("./yzzfs.new","wb");

    if(fp_write==NULL)
    {
        printf("open yzzfs.new failed!\n");
        return 2;
    }

    ret=fread(buf,sizeof(unsigned char),2048,fp_read);

    printf("previous magic number is 0x%02x%02x\n",buf[0x439],buf[0x438]);

    buf[0x439]=0x04;
    buf[0x438]=0x0c;

    fwrite(buf,sizeof(unsigned char),2048,fp_write);

    printf("current magic number is 0x%02x%02x\n",buf[0x439],buf[0x438]);

    while(ret == 2048)
    {
        ret=fread(buf,sizeof(unsigned char),2048,fp_read);
        fwrite(buf,sizeof(unsigned char),ret,fp_write);
    }

    if(ret < 2048 && feof(fp_read))
    {
        printf("done changing magic number\n");
    }

    fclose(fp_read);
    fclose(fp_write);

    return 0;
}
