#if !defined(COMMAND_H)
#define COMMAND_H

enum CommandFlag {
    COMMAND_FLAG_NONE = 0,
    COMMAND_FLAG_CAPTURE_STDOUT = 1 << 0,
    COMMAND_FLAG_CAPTURE_STDERR = 1 << 1,
    COMMAND_FLAG_MERGE_STDERR = 1 << 2,
    COMMAND_FLAG_ASYNC = 1 << 3,
    COMMAND_FLAG_DETACHED = 1 << 4,
    COMMAND_FLAG_NEW_SESSION = 1 << 5,
    COMMAND_FLAG_NEW_PROCESS_GROUP = 1 << 6,
    COMMAND_FLAG_STDIN_TTY = 1 << 7,
    COMMAND_FLAG_CLOSE_STDIN = 1 << 8,
    COMMAND_FLAG_LAST,
};

typedef struct CommandResult {
    char *output;
    char *stdout_output;
    char *stderr_output;

    int64 pid;

    int32 output_len;
    int32 stdout_len;
    int32 stderr_len;
    int32 status;
    int32 error_status;
    int32 exit_status;
    int32 term_signal;
    int32 stdout_fd;
    int32 stderr_fd;

    bool exited;
    bool signaled;
    bool stdout_fd_open;
    bool stderr_fd_open;
} CommandResult;

typedef struct Command {
    char **argv;
    char **env;
    char *cwd;

    int32 *argvs_lens;
    int32 *env_lens;
    int32 cwd_len;
    int32 argc;
    int32 env_len;
    int32 cap;
    int32 env_cap;
    int32 error_status;

    CommandResult result;
} Command;

static char *COMMAND_FLAG_str(enum CommandFlag);
static void COMMAND_FLAG_str_free(char *);
static enum CommandFlag COMMAND_FLAG_parse(char *);
static bool command_flag_token_equal(char *, int32, char *);
static void command_argv0_set(Command *, char *);
static void command_child_env_apply(Command *);
static void command_child_exec(
    Command *, enum CommandFlag, int [2], int [2]
) __attribute__((noreturn));
static void command_cwd_clear(Command *);
static void command_cwd_set(Command *, char *);
static void command_env_clear(Command *);
static void command_env_printf(Command *, char *, ...);
static void command_env_push(Command *, char *);
static void command_env_push_length(Command *, char *, int32);
static void command_error_set(Command *, int32);
static bool command_flags_capture(enum CommandFlag);
static bool command_flags_capture_stderr(enum CommandFlag);
static bool command_flags_capture_stdout(enum CommandFlag);
static enum CommandFlag command_flags_normalized(enum CommandFlag);
static void command_free(Command *);
static void command_print(Command *);
static void command_printf(Command *, char *, ...);
static void command_push(Command *, char *);
static void command_push_length(Command *, char *, int32);
static void command_push_owned_length(
    char ***,
    int32 **,
    int32 *,
    int32 *,
    char *,
    int32
);
static void command_push_split(Command *, char *, char *);
static void command_reset(Command *);
static void command_result_append(
    StrBuilder *,
    StrBuilder *,
    StrBuilder *,
    bool,
    char *,
    int32
);
static void command_result_file_descriptors_close(CommandResult *);
static void command_result_free(CommandResult *);
static void command_result_init(CommandResult *);
static void command_result_read_captured(Command *);
static bool command_run(Command *, enum CommandFlag);
static bool command_run_async(Command *, enum CommandFlag);
static bool command_run_capture(Command *, enum CommandFlag);
static bool command_run_capture_all(Command *);
static bool command_run_capture_combined(Command *);
static bool command_run_sync(Command *, int *);
static bool command_signal(Command *, int32, bool);
static bool command_start(Command *, enum CommandFlag);
static int32 command_status_from_wait(int, CommandResult *);
static char *command_str(Command *, int32 *);
static void command_vector_reserve(char ***, int32 **, int32 *, int32, int32);
static bool command_wait(Command *);

#define COMMAND_PUSH_2(A, B) command_push(A, B)
#define COMMAND_PUSH_3(A, B, B_LEN) command_push_length(A, B, B_LEN)
#define COMMAND_PUSH(...) SELECT_ON_NUM_ARGS(COMMAND_PUSH_, __VA_ARGS__)

#define COMMAND_ENV_PUSH_2(A, B) command_env_push(A, B)
#define COMMAND_ENV_PUSH_3(A, B, B_LEN) \
    command_env_push_length(A, B, B_LEN)
#define COMMAND_ENV_PUSH(...) \
    SELECT_ON_NUM_ARGS(COMMAND_ENV_PUSH_, __VA_ARGS__)

#endif /* COMMAND_H */
