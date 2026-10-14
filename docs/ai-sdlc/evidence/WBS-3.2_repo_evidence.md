# WBS-3.2 repository evidence pack

This file records the repository evidence used to finalize the WBS-3.2 AI-assisted debugging discipline case studies.

## Source files inspected

- .github/copilot-instructions.md
- docs/ai-sdlc/WORKFLOW_MODEL_v1.md
- docs/ai-sdlc/PROMPT_ROUTER.md
- docs/ai-sdlc/handovers/WBS-3.1_summary.md
- REQUIREMENTS.md
- src/transport.cpp
- src/message_router.cpp
- include/someip/service.hpp
- tests/test_transport_retry_backoff.cpp
- tests/test_message_router_error.cpp
- tests/test_transport_stress.cpp

## Issue evidence

### Issue #2
Title: [Defect] UDP Request Timeout & Retry Resilience in UdpEndpoint

Relevant requirements:
- REQUIREMENTS.md: RQ-02-001
- REQUIREMENTS.md: RQ-08-003

Evidence statements:
- The repo includes retry/backoff behavior in src/transport.cpp.
- The regression test tests/test_transport_retry_backoff.cpp checks retry behavior and timing.

### Issue #3
Title: [Bug] Concurrent State Data Race & Silent Drop on Unregistered Methods

Relevant requirements:
- REQUIREMENTS.md: RQ-03-003
- REQUIREMENTS.md: RQ-08-001
- REQUIREMENTS.md: RQ-13-003

Evidence statements:
- send_error logic exists in src/message_router.cpp.
- the router checks missing handlers before sending an error response.
- ServiceRegistry is mutex-protected in include/someip/service.hpp.
- transport state synchronization exists in src/transport.cpp.
- stress regression exists in tests/test_transport_stress.cpp.

## Evidence quality statement

This evidence pack is based on repository state inspection only. It does not claim fresh runtime execution was performed in this session.

The repo state shows that the issues are already represented in code and test artifacts. The task is therefore best interpreted as a case-study reconstruction for WBS-3.2 rather than a live failing-build debug session.

## Interpretation

- FACT: the issue descriptions, current code, and tests are all present in the repo.
- FACT: the current branch already contains the fix paths and regression tests.
- INFERRED: the combined Issue #3 path is the most likely AI-introduced or high-risk defect candidate.
- UNKNOWN: actual pass/fail logs from executing the tests in a local or CI environment.

## Final status

WBS-3.2 has been finalized as a repo-evidence case-study deliverable and evidence set.

