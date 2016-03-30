/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: krati
 *
 * Created on March 16, 2016, 8:20 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <uuid/uuid.h>
#include <stdbool.h>
#include<string.h>
/*
 * 
 */


/* The current WARC file (or NULL, if WARC is disabled). */
 static FILE *warc_current_file;
 
 /* This is true until a warc_write_* method fails. */
static bool warc_write_ok;
 
 typedef long wgint;
 
 
 
 
char *
warc_timestamp (char *timestamp, size_t timestamp_size)
{
  time_t rawtime = time (NULL);
  struct tm * timeinfo = gmtime (&rawtime);

  if (strftime (timestamp, timestamp_size, "%Y-%m-%dT%H:%M:%SZ", timeinfo) == 0 && timestamp_size > 0)
    *timestamp = 0;

  return timestamp;
}

void
warc_uuid_str (char *urn_str)
{
  char uuid_str[37];
  uuid_t record_id;

  uuid_generate (record_id);
  uuid_unparse (record_id, uuid_str);

  sprintf (urn_str, "<urn:uuid:%s>", uuid_str);
}


static size_t
warc_write_buffer (const char *buffer, size_t size)
{

    return fwrite (buffer, 1, size, warc_current_file);
}

static bool
warc_write_string (const char *str)
{
  size_t n;

  n = strlen (str);
  if (n != warc_write_buffer (str, n))
    warc_write_ok = false;

  return warc_write_ok;
}


static bool
warc_write_header (const char *name, const char *value)
{
  if (value)
    {
      warc_write_string (name);
      warc_write_string (": ");
      warc_write_string (value);
      warc_write_string ("\r\n");
    }
  return true;
}

static bool
warc_write_date_header (const char *timestamp)
{
  char current_timestamp[21];

  return warc_write_header ("WARC-Date", timestamp ? timestamp :
                            warc_timestamp (current_timestamp, sizeof(current_timestamp)));
}


/* Copies the contents of DATA_IN to the WARC record.
   Adds a Content-Length header to the WARC record.
   Run this method after warc_write_header,
   then run warc_write_end_record. */
static bool
warc_write_block_from_file (FILE *data_in)
{
  /* Add the Content-Length header. */
  char content_length[((sizeof(off_t) * 24082 / 10000) + 2)];
  char buffer[BUFSIZ];
  size_t s;

  fseeko (data_in, 0L, SEEK_END);
//  number_to_string (content_length, ftello (data_in));
//  warc_write_header ("Content-Length", content_length);

  /* End of the WARC header section. */
 // warc_write_string ("\r\n");

  if (fseeko (data_in, 0L, SEEK_SET) != 0)
    warc_write_ok = false;

  /* Copy the data in the file to the WARC record. */
  while ((s = fread (buffer, 1, BUFSIZ, data_in)) > 0)
    {
      if (warc_write_buffer (buffer, s) < s)
        warc_write_ok = false;
    }

  return warc_write_ok;
}

bool
warc_write_response_record (FILE *body, const char *timestamp_str)
{
 
  char response_uuid [48];
  off_t offset;
  char c;

      rewind (body);
       
  /* Not a revisit, just store the record. */

  warc_uuid_str (response_uuid);
  warc_current_file = fopen ("test.warc", "wb+");
  fseeko (warc_current_file, 0L, SEEK_END);
  offset = ftello (warc_current_file);

  
  warc_write_header ("WARC-Type", "response");
  warc_write_header ("WARC-Record-ID", response_uuid);

  warc_write_date_header (timestamp_str);
 
  warc_write_header ("Content-Type", "application/http;msgtype=response");
  rewind(body);
//  if (body) {
//    while ((c = getc(body)) != EOF)
//        printf("%c",c);
//    
//}
//  
//    rewind(body);
  warc_write_block_from_file (body);
  
    warc_write_buffer ("\r\n\r\n", 4);
 fclose (warc_current_file);

  fclose (body);

  return warc_write_ok;
}

int main(int argc, char** argv) {
    
    FILE *body=fopen ("/home/krati/test.txt", "rb+");
  
    char *timestamp = (char *)malloc(22*sizeof(char));
    char *ans,uuid[48];
    
    printf("Hello World!");


    ans=  warc_timestamp(timestamp,22);
    
      printf("%s",ans);
      
      warc_uuid_str(uuid);
     printf("%s",uuid);
     warc_write_response_record(body,timestamp);
 return 0;   
}

