#define	MAX_NO_OF_REMOTES 256
#define MAX_MACHINE_LENGTH 30
typedef struct{
	int	no_of_slaves;
	char	slave_names[MAX_NO_OF_REMOTES][MAX_MACHINE_LENGTH];
	int	slave_fp[MAX_NO_OF_REMOTES];
	int	slave_err_fp[MAX_NO_OF_REMOTES];
	FILE	*slave_r_file_des[MAX_NO_OF_REMOTES];
	FILE	*slave_w_file_des[MAX_NO_OF_REMOTES];
	FILE	*slave_err_des[MAX_NO_OF_REMOTES];
	int	process_running[MAX_NO_OF_REMOTES];
	StorageLink	*slave_storage_link[MAX_NO_OF_REMOTES];
}Slaves;

#define MAX_PROCESSES 256
#define	P_READ_ONLY 0
#define	P_WRITE_ONLY 1
#define	P_READ_WRITE 2
#define	SLAVE_LIST_LENGTH 5000
#define SLAVE_STATUS_OK_MSG "slave_status_ok"
#define SLAVE_TERMINATION_MSG "slave_terminate_stream"
#define MASTER_TERMINATION_MSG " master_terminate_stream "
