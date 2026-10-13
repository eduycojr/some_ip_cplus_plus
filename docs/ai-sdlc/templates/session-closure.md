# Session Closure Template

Use this template to record closure, evidence, and handover to the next stage.

Session ID: S[YYYYMMDD]-[WBS]-[NN]
WBS scope: [IDs]
Objective: [outcome]

Files changed / created (actual):
- path: <path> commit: <sha> diff: <link>

Commands executed (with environment):
- <command> | env: <os/toolchain> | log: docs/ai-sdlc/evidence/<logfile>.log

Verification evidence attached:
- Build log: docs/ai-sdlc/evidence/build_<sha>.log
- Test logs: docs/ai-sdlc/evidence/tests_<sha>.log
- CI artifact: <url>

Acceptance criteria mapping:
- Criterion 1: PASSED / FAILED / BLOCKED / NOT_RUN — evidence: <link>
- Criterion 2: PASSED / ...

Learning-loop entries referenced:
- docs/ai-sdlc/evidence/learning_loops.md — entries: [IDs]

Manifest update:
- manifest.yaml entry added/updated for: [artifact id]

Next-stage required inputs (exact paths + SHAs):
- [path] commit: <sha>

Closure status: Closed / Closed with open items / Blocked / Interrupted-recovery required

Handover author: @username
Handover commit: <sha>
