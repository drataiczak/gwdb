#include <callback.h>

void on_ready_cb(struct discord *client, const struct discord_user *bot) {
  log_info("Logged in as %s. Listening for messages...", bot->username);
}
