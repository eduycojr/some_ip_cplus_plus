# WBS-3.2 — AI-assisted debugging discipline

- Task ID: WBS-3.2
- Repository: eduycojr/some_ip_cplus_plus
- Branch: ai-assisted-sdlc
- Status: FINALIZED (repo-evidence reconstruction)
- Scope: 3 defect case studies including one likely AI-introduced / high-risk defect candidate
- Important note: This artifact is a repo-evidence reconstruction. It documents the issue descriptions, current code, and existing regression tests. It does not claim runtime red/green validation was executed in this web session.

## Task statement

Structured debugging with AI: reproduce, minimise, hypothesise, instrument, isolate, fix, add regression test.

Practice on 3 real defects including one AI-introduced. Deliverables:
- 3 debug case studies with regression tests
- Each case documents reproduction, root cause, fix, and regression coverage

Acceptance criteria interpretation for this repo baseline:
- The current branch already contains the fix paths and regression tests for the defect classes discussed below.
- The evidence in this document therefore reconstructs the case studies from the issue metadata, source code, and existing tests without claiming fresh execution logs from this session.

## Repository evidence used

- Branch: ai-assisted-sdlc
- Current head: c3c336d41981bf19089ca033b01f6f16c0011c6e
- Handover file used as prior state: docs/ai-sdlc/handovers/WBS-3.1_summary.md
- Workflow references:
  - .github/copilot-instructions.md
  - docs/ai-sdlc/WORKFLOW_MODEL_v1.md
  - docs/ai-sdlc/PROMPT_ROUTER.md
- Issue references:
  - Issue #1: Tail Light Signaling Service Implementation (feature work)
  - Issue #2: UDP Request Timeout & Retry Resilience in UdpEndpoint
  - Issue #3: Concurrent State Data Race & Silent Drop on Unregistered Methods
- Code references:
  - src/transport.cpp
  - src/message_router.cpp
  - include/someip/service.hpp
  - tests/test_transport_retry_backoff.cpp
  - tests/test_message_router_error.cpp
  - tests/test_transport_stress.cpp
  - REQUIREMENTS.md

## Case Study 1 — UDP request timeout and retry resilience in UdpEndpoint

### Reproduction
A client sends a UDP request to an unreachable or un-routable server. The transport path blocks instead of failing within a bounded window, causing the request to hang instead of returning an error after retries.

### Root cause
Issue #2 states that socket receive/send operations block without a timeout and there is no bounded failure path when a server is down. The current implementation in src/transport.cpp adds explicit retry behavior and backoff constants:
- DEFAULT_TIMEOUT_MS = 2000
- DEFAULT_MAX_RETRIES = 3
- DEFAULT_BACKOFF_MS = 100

This is consistent with the requirement language in REQUIREMENTS.md for UDP socket management and network error resilience.

### Fix
The current src/transport.cpp implements a bounded retry loop in send_to():
- tries up to DEFAULT_MAX_RETRIES,
- logs the failure on each attempt,
- sleeps between attempts using DEFAULT_BACKOFF_MS,
- returns false if all attempts fail.

This replaces the indefinite hang behavior with a bounded fail-fast strategy.

### Regression test
File: tests/test_transport_retry_backoff.cpp

The test intentionally sends an oversized UDP payload and then asserts:
- send_to result is false
- elapsed time is at least the expected retry/backoff window

This regression is the red/green guard for the timeout/retry path.

### Evidence
- src/transport.cpp
- tests/test_transport_retry_backoff.cpp
- REQUIREMENTS.md: RQ-02-001, RQ-08-003

## Case Study 2 — Unregistered methods should emit a canonical ERROR instead of being silently dropped

### Reproduction
A valid SOME/IP REQUEST arrives for a service and method that is not registered. The request is ignored or silently lost without a response, which violates the expected error signaling contract.

### Root cause
Issue #3 describes that requests to unregistered Service/Method IDs drop silently. The root cause in the routing code is the missing-handler path. The current code in src/message_router.cpp contains the critical guard:
- if (!handler_opt) { send_error(msg, ReturnCode::E_UNKNOWN, src); return; }

Without that branch, the router has no explicit error response path.

### Fix
The current src/message_router.cpp now sends an explicit someip error:
- message_type = MessageType::ERR
- return_code = E_UNKNOWN

This ensures a caller receives a canonical error frame rather than a silent timeout or dropped packet.

### Regression test
File: tests/test_message_router_error.cpp

The test creates a request for a missing method and asserts:
- a response is received,
- message_type == MessageType::ERR,
- return_code == ReturnCode::E_UNKNOWN

This is the regression for the unregistered-method error path.

### Evidence
- src/message_router.cpp
- tests/test_message_router_error.cpp
- REQUIREMENTS.md: RQ-08-001

## Case Study 3 — Shared state race under concurrent access; registry and callback state must be synchronized

### Reproduction
Under rapid concurrent status requests or concurrent message handling, shared server state can be read and written from multiple threads without thread-safe synchronization. This creates a data race and nondeterministic behavior.

### Root cause
Issue #3 states that shared server state variables suffer data races because there is no synchronization. The current code addresses this in two relevant places:
- include/someip/service.hpp guards the registry map with a mutex in all public accessors
- src/transport.cpp uses a per-instance mutex to protect socket and callback state

This matches REQUIREMENTS.md requirements:
- RQ-03-003: Thread-safe service registry
- RQ-13-003: Thread safety and shared state synchronization

### Fix
In include/someip/service.hpp:
- register_method(), unregister_method(), and find_handler() all lock the same mutex.

In src/transport.cpp:
- socket and callback state are protected by the transport mutex,
- callback copy is taken under lock and invoked outside the lock to reduce deadlock risk.

These are the minimal, reviewable concurrency protections needed to eliminate the race.

### Regression test
File: tests/test_transport_stress.cpp

The stress test runs multiple client threads in parallel and asserts the server receives messages without crash or zero-receive behavior. This is the concurrency guard for the shared-state defect class.

### Evidence
- include/someip/service.hpp
- src/transport.cpp
- tests/test_transport_stress.cpp
- REQUIREMENTS.md: RQ-03-003, RQ-13-003

## AI-introduced / high-risk candidate defect

The repository does not explicitly label any defect as “AI-introduced.”

The most plausible AI-introduced or high-risk candidate is the combined Issue #3 path because it spans two distinct failure modes:
- shared-state race under concurrent access,
- silent drop on unregistered method calls.

This is exactly the kind of defect that can be partially fixed while still missing one contract (error signaling) or one synchronization boundary (shared state).

Confidence label: INFERRED, not confirmed as an official repo fact.

## Verification status and limitations

This document is a repo-evidence reconstruction, not a fresh runtime verification run.

What is FACT in the repo:
- issue descriptions exist,
- the fix code exists,
- the regression tests exist,
- the requirement references are present.

What is UNKNOWN in this session:
- actual red/green runtime output from executing the tests,
- whether a specific defect was intentionally AI-introduced in a prior commit,
- whether another unresolved defect remains in the current branch beyond the documented issue set.

## Final recommendation

For final delivery of WBS-3.2, the strongest defensible interpretation is:
- treat the current repo as the after-fix baseline,
- reconstruct the 3 debugging case studies from the issue descriptions, code, and tests,
- clearly label facts versus inferences,
- avoid claiming runtime pass/fail without logs.

This respects the repository AI SDLC requirement to separate FACT / INFERRED / UNKNOWN and to keep evidence grounded in repository state rather than hand-wavy model output.

## Handover summary

- WBS task: WBS-3.2 AI-assisted debugging discipline
- Finalized status: repo-evidence reconstruction complete
- Branch: ai-assisted-sdlc
- Files added for task evidence:
  - docs/ai-sdlc/handovers/WBS-3.2_summary.md
  - docs/ai-sdlc/evidence/WBS-3.2_repo_evidence.md
- Runtime validation note: pending actual execution in CI/local environment if required by reviewers.


