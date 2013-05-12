typedef unsigned char byte;

class SkyltController {
public:

  // Parent process interface
  static SkyltController* GetController();

  void Start(int inp);
  void Restart();
  static void Kill();

  // Shared interface
  ~SkyltController();

private:

  // Parent process variables
  static void HUPHandler(int);
  int m_pid;

  static SkyltController m_inst;

  // Child process variables
  SkyltController();
  void Run();

  int m_inp;
  int m_nBS;
  byte* m_pSkylt;

  enum { MODE_1BIT = 1, MODE_4BIT = 3 };
  enum { SETMODE = 1, SLEEP = 2, SETFRAME = 3 };

  int isChild() { return m_pid == 0; };
};
