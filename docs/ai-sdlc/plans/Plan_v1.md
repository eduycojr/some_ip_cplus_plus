# WBS-2.4 — Plan v1: Implementation Plan for ISSUE-01, ISSUE-02, ISSUE-03

## Purpose
- Convert the approved WBS-2.2 requirement decomposition and the validated WBS-2.3 repo map into an implementation-ready plan.
- Identify remaining risks, required tests, and decision gates before coding begins.

## Source baseline
- Requirement decomposition: `docs/ai-sdlc/evidence/W2-2.2_Requirement_Decomposition_Examples.md`
- Repository map: `docs/ai-sdlc/handovers/WBS-2.3_execution_evidence.md`
- Runtime test evidence: `docs/ai-sdlc/evidence/tests_967d268.log`
- Core implementation files: `include/someip/*.hpp`, `src/*.cpp`, `examples/*.cpp`, `tests/*.cpp`

## Scope
- ISSUE-01: Tail Light Signaling Service / stateful example feature
- ISSUE-02: UDP timeout & retry resilience
- ISSUE-03: Concurrent state data race & silent drop handling

## Phase 0 — entry gates and controls
- Gate G0.1: WBS-2.2 approved and recorded in manifest
- Gate G0.2: WBS-2.3 runtime evidence exists: tests_967d268.log shows 5/5 passing tests
- Gate G0.3: No human-approved implementation plan exists yet; this plan becomes the authoritative baseline for implementation work
- Required human decisions:
  - approve plan v1 or request modifications
  - approve challenge log findings and plan v2 decisions

## Workstreams

### Workstream A — ISSUE-01: Tail Light Signaling Service
- Files likely involved:
  - `include/someip/types.hpp`
  - `include/someip/service.hpp`
  - `src/message_router.cpp`
  - `examples/taillight_server_app.cpp`
  - `examples/taillight_client_app.cpp`
  - `CMakeLists.txt`

#### Tasks
1. Add Tail Light service constants and message mapping to the public type definitions.
2. Add standalone example server/client for taillight service.
3. Implement stateful hazard/on/off behavior without breaking the existing brake demo.
4. Add end-to-end test cases for tail light request/reply and malformed requests.
5. Verify port isolation and import compatibility.

#### Acceptance checks
- Service ID 0x1400 and method IDs are registered without collision
- request/response formats match the approved binary contract
- hazard state is independent from on/off state
- error cases return correct error response type

### Workstream B — ISSUE-02: UDP timeout & retry resilience
- Files likely involved:
  - `include/someip/transport.hpp`
  - `src/transport.cpp`
  - `include/someip/types.hpp`
  - `src/message_router.cpp`
  - `tests/test_transport_retry_backoff.cpp`

#### Tasks
1. Add timeout and retry configuration options to the transport API.
2. Add timeout configuration to socket setup and receive loop.
3. Add fixed retry/backoff logic with bounded attempts.
4. Map `E_TIMEOUT` handling into return codes and message routing.
5. Add tests for default timeout values and custom timeout setters.

#### Acceptance checks
- Default timeout value is present and applies when unset
- custom setter works and is backward compatible
- retry loop is bounded at 3 attempts
- final failure returns the correct error payload

### Workstream C — ISSUE-03: Data race + silent drop handling
- Files likely involved:
  - `examples/server_app.cpp`
  - `src/message_router.cpp`
  - `include/someip/service.hpp`
  - `tests/test_transport_stress.cpp`
  - `CMakeLists.txt`

#### Tasks
1. Audit shared mutable state and identify lock scope.
2. Introduce synchronization for stateful handlers.
3. Add explicit null-endpoint and send_error logging.
4. Add concurrency tests using TSan/ASan-capable build settings and a multi-threaded scenario.
5. Validate no deadlocks or silent drops remain.

#### Acceptance checks
- Shared state protected by mutex or atomic where needed
- concurrency tests observe zero data races under sanitizer build
- unregistered methods produce explicit error packet with echoed identifiers
- deadlock/hang conditions are excluded in concurrent request scenarios

## Implementation sequence
### Sprint 1 - Core safety and validation
1. Implement ISSUE-02 timeout/retry path first because it most directly alters transport behavior and has clear acceptance tests.
2. Implement ISSUE-03 synchronization and logging path second due to correctness and race risk.
3. Implement ISSUE-01 tail light example last because it is feature-oriented and uses established error/state patterns.

This order reduces the chance of later rework because transport and synchronization issues are the most critical correctness risks.

## Test strategy
- Unit tests:
  - serialization contract tests
  - timeout config tests
  - retry loop tests
  - error code tests
- End-to-end tests:
  - request/reply path for service calls
  - error path for unknown methods
  - concurrent send/receive path
- Sanitizer runs:
  - ASan / UBSan / TSan if a Linux or Clang-enabled environment is used
- Coverage target:
  - >=80% line coverage on changed code for WBS-2.7

## Risk register
1. Hidden API compatibility break in transport or registry build surfaces
2. Timeout/retry semantics may change message behavior under load
3. Concurrency fixes may introduce lock-order or deadlock changes
4. Example apps may drift from baseline behavior in unrelated demos
5. Incomplete requirement approval could cause rework after code is written

## Deliverables for implementation phase
- Branch with implemented CODE changes
- Tests to verify each issue
- Updated documentation or README additions where required
- PR with per-hunk rationale and test evidence
- Final handover from implementation to verification

## Exit gate for WBS-2.4
Plan v1 is approved when:
- the challenge log is recorded and reviewed
- each issue is assigned a mitigation decision (fix / accept / defer)
- required tests and runtime evidence are identified
- the initial implementation order is approved by the human reviewer

