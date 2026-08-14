/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2026
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "plugin.h"

#define CHECKPOINT_PATH ROCKBOX_DIR "/podcast_checkpoint.txt"
#define CHECKPOINT_TMP  ROCKBOX_DIR "/podcast_checkpoint.txt.tmp"

static void write_checkpoint_for_current_track(void)
{
    struct mp3entry *id3 = rb->audio_current_track();
    if (id3 == NULL || id3->path[0] == '\0')
        return;

    int fd = open(CHECKPOINT_TMP, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
        return;

    char text[512];
    int n = rb->snprintf(text, sizeof(text),
                         "%s\n%lu\n%lu\n",
                         id3->path,
                         id3->elapsed,
                         id3->length);
    if (n > 0)
        write(fd, text, n);

    close(fd);
    rename(CHECKPOINT_TMP, CHECKPOINT_PATH);
}

enum plugin_status plugin_start(const void *parameter)
{
    (void)parameter;

    rb->splash(HZ * 2, "Podcast checkpoint");

    if (rb->audio_status() & AUDIO_STATUS_PLAY)
        write_checkpoint_for_current_track();

    return PLUGIN_OK;
}
