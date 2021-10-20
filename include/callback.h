#ifndef CALLBACK_H
#define CALLBACK_H

#include <orca/discord.h>

/* Runs when the bot is initated */
void on_ready_cb(struct discord *, const struct discord_user *);

/* Runs when a message is generated in the channel the bot is a part of */
void on_message_cb(struct discord *, const struct discord_user, const struct discord_message *);


#endif
