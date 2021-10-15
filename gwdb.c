#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <orca/discord.h>
#include "lib/emojis.h"

#define MSG_BUFF_SIZE 1024
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
