#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <orca/discord.h>

#include <curl/curl.h>

#include "lib/emojis.h"
#include "lib/gw2_api.h"

#define MSG_BUFF_SIZE 1024

void curl_handler(void *ptr, size_t size, size_t nmemb, void *stream) {
  log_info("%s", ptr);
}
void on_message(struct discord *client, 
                const struct discord_user *bot, 
                const struct discord_message *msg)
{
  char *msg_buffer = calloc(1, 256);
  if(NULL == msg_buffer) {
    log_info("[!] Failed to create message buffer");
     return;
  }

  snprintf(msg_buffer, 
           MSG_BUFF_SIZE, 
           "Hello, %s#%s.",
           msg->author->username,
           msg->author->discriminator);

  struct discord_create_message_params params = { 
    .content = msg_buffer
  };

  /* Don't respond to ourselves */	
  if(msg->author->bot || msg->referenced_message) {
    return;
  }

  /* Thumbsup and say "Hello <user>#<discriminator> */
  discord_create_reaction(client, msg->channel_id, msg->id, 0, THUMBSUP);
  discord_create_message(client, msg->channel_id, &params, NULL);

  if(0 == strcmp("!pvedaily", msg->content)) {
    CURL *curl = curl_easy_init();

    if(curl) {
      CURLcode resp;

      char *url = NULL;
      url = calloc(1, 256);
      if(NULL == url) {
        log_info("[!] Unable to calloc URL buffer");
        return;
      }

      strcpy(url, BASE_URL);
      strcat(url, API_VERS);
      strcat(url, "/");
      strcat(url, ACHIEVEMENTS_ENDPOINT);
      strcat(url, "/daily");

      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_handler);
      curl_easy_setopt(curl, CURLOPT_URL, url);
      resp = curl_easy_perform(curl);

      curl_easy_cleanup(curl);
    }
  }

  free(msg_buffer);
}

void on_ready(struct discord *client, 
              const struct discord_user *bot) 
{
  log_info("Logged in as %s!", bot->username);
}

int main() {
  struct discord *client = discord_config_init("config.json");

  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_create(client, &on_message);
  discord_run(client);

  discord_cleanup(client);
  
  return 0;
}
