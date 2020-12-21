If Just-In-Time debugging doesn't start when an app crashes, even though it is enabled in Visual Studio:

- Windows Error Reporting could be taking over the error handling on your computer.

  To fix this issue, use Registry Editor to add a **DWORD Value** of **Disabled**, with **Value data** of **1**, to the following registry keys:

  - **HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\Windows Error Reporting**
  - (For 64-bit machines): **HKEY_LOCAL_MACHINE\Software\WOW6432Node\Microsoft\Windows\Windows Error Reporting**

  For more information, see [.WER settings](https://docs.microsoft.com/en-us/windows/desktop/wer/wer-settings).

- A known Windows issue may be causing the Just-In-Time debugger to fail.

  The fix is to add a **DWORD Value** of **Auto**, with **Value data** of **1**, to the following registry keys:

  - **HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug**
  - (For 64-bit machines): **HKEY_LOCAL_MACHINE\Software\WOW6432Node\Microsoft\Windows NT\CurrentVersion\AeDebug**



## ref

1. [Troubleshoot Just-In-Time debugging](https://docs.microsoft.com/en-us/visualstudio/debugger/debug-using-the-just-in-time-debugger?view=vs-2019#jit_errors)