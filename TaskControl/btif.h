/*
 * library function to display bittorrent files, 
 * download progress and instrument bittorrent
 * daemon
 */

#ifndef BTIF_H
#define BTIF_H

#ifdef __cplusplus
extern "C" {
#endif

enum bt_state {
    BT_STATE_INACTIVE,
    BT_STATE_START,
    BT_STATE_STOP,
    BT_STATE_LEECH,
    BT_STATE_SEED,
    BT_STATE_ERROR
};

/* define ERROR code returns from bt */
#define BT_ERR_FILE     -1  /* file is not torrent format */
#define BT_ERR_COMM     -2  /* communication failed with btpd */
#define BT_ERR_DIR      -3  /* download dir error (readonly?) */
#define BT_ERR_EXIST    -4  /* add a torrent already in btpd */
#define BT_ERR_MEM      -5  /* memory failure in malloc */
#define BT_ERR_ID	-6  /* supplied ID is wrong */
#define BT_ERR_OTHER    -7  /* unknown error */

struct bt_item {
  unsigned int num;  /* torrent job index num */
  char * name;
  enum bt_state st;
  long long cgot, csize, totup; /* bytes uploaded/downloaded */
  long long dwrate, uprate; /* upload/download rate, bytes/s */
  unsigned int npeers;      /* how many peers are there? */
  struct bt_item * next;
};

struct bt_jobitems {
  unsigned int cnt; 
  struct bt_item * head;
};

struct bt_fileinfo {
  char * dir;
  unsigned int cnt;
  struct bt_item * head;
};

void bt_free_jobs (struct bt_jobitems *job);
void bt_free_file (struct bt_fileinfo *f);

/* 
 * return the list of files to be downloaded in 
 * a torrent file. Only filename and dir name 
 * is relevant in the items returned.
 * 
 * return ptr to item if successful,
 *        NULL if error
 */
extern struct bt_fileinfo* bt_torrent_info (char* filen);

extern struct bt_fileinfo * bt_torrent_id (int id, int *err);

/*
 * add a torrent file to the current download jobs
 * 
 * return 0 if successful
 *       -1 if error
 */

extern int bt_torrent_add (char *filen);


/*
 * list the running torrent jobs, if (id<0), all jobs are listed.
 * otherwise, the job with specified id is listed
 */
extern struct bt_jobitems * bt_torrent_list (int id);

/*
 * pause a torrent job, using torrent index
 */
extern int bt_torrent_pause (unsigned long index);

/*
 * start a paused torrent job.
 */
extern int bt_torrent_start (unsigned long index);

/*
 * delete a torrent job, without deleting files
 */
extern int bt_torrent_del (unsigned long index);

/*
 * delete a torrent job, and its files 
 */
extern int bt_torrent_del_taskfile (unsigned long index);

/*
 * kill bt daemon
 */
extern int bt_torrent_kill (int sec);

extern int bt_torrent_ver (void);

#ifdef __cplusplus
}
#endif

#endif
