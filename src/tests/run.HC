#include "run_shared.HC"

U0 RunTest(U8 *filename)
{
  U8 buffer[512];
  I64 len;

  /* Use the local build (../../hcc relative to src/tests/) so we
   * always test what's actually been compiled, not whatever older
   * hcc happens to be on PATH (e.g. an installed /usr/local/bin/hcc
   * from a prior version - this trap cost us a session of confused
   * cross-test triage). */
  U8 install_flag[512];
  install_flag[0] = '\0';
  if (g_install_dir[0] != '\0') {
    len = snprintf(install_flag, sizeof(install_flag),
                   " --install-dir=%s", g_install_dir);
    install_flag[len] = '\0';
  }

  if (g_target[0] == '\0') {
    len = snprintf(buffer, sizeof(buffer), "../../hcc%s %s",
                   install_flag, filename);
  } else {
    len = snprintf(buffer, sizeof(buffer),
                   "../../hcc --target=%s --install-dir=../../build/%s %s",
                   g_target, g_target, filename);
  }
  buffer[len] = '\0';
  auto res = System(buffer);

  if (res != 0) {
    "\033[0;31mFailed to compile\033[0;0m: %s\n",filename;
  } else {
    if ((res = System("./a.out")) != 0) {
      "\033[0;31mFAILED\033[0;0m to run: %s\n",filename;
    }
    Rm("./a.out");
  }
}

I32 Main(I32 argc, U8 **argv)
{
  /* Argv contract:
   *   ./run                     -> host arch, all tests
   *   ./run --target=<triple>   -> cross to <triple>, all tests
   *   ./run <file.HC>           -> host arch, single file
   *   ./run --target=<triple> <file.HC>
   *                             -> cross to <triple>, single file
   *
   * Disambiguation is by the --target= prefix so the existing
   * single-file workflow keeps working unchanged. */
  g_target = "";
  g_install_dir = "";
  I64 arg_idx = 1;
  while (arg_idx < argc && StrNCmp(argv[arg_idx], "--", 2) == 0) {
    if (StrNCmp(argv[arg_idx], "--target=", 9) == 0) {
      g_target = argv[arg_idx] + 9;
    } else if (StrNCmp(argv[arg_idx], "--install-dir=", 14) == 0) {
      g_install_dir = argv[arg_idx] + 14;
    }
    arg_idx++;
  }

  if (argc > arg_idx) {
    RunTest(argv[arg_idx]);
    Exit(0);
  }

  PtrVec *vec = CollectTests;

  for (U8 *entry : vec) {
    RunTest(entry);
  }

  PtrVecRelease(vec, &Free);
  return 0;
}
