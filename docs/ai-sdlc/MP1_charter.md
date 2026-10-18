# MP1 Charter — AI‑Assisted SDLC Mini Project 1 (MP1)

Session ID: S20261013-MP1-01
Project: some_ip_cplus_plus
Branch: ai-assisted-sdlc
Author: AI Assistant (draft), finalized by @eduycojr

## Objective
Deliver a working feature increment and a bug fix (MP1) that demonstrates the AI‑assisted SDLC workflow end‑to‑end: requirements, design, implementation, tests, reviews, and measured productivity change in an unfamiliar open-source repository.

## Scope
This MP1 covers the approved requirement baseline for the three issues listed in WBS-2.2:
- ISSUE‑01: Tail Light Signaling Service (example binaries) — implement standalone server/client examples, add service constants, and end‑to‑end tests.
- ISSUE‑02: UDP Timeout & Retry Resilience — implement timeout/retry configuration in transport layer and tests.
- ISSUE‑03: Concurrent State Data Race & Routing Fix — add synchronization, logging for null endpoints, and concurrency tests.

Implementation will be delivered in two increments:
- Increment 1 (WBS‑2.6): implement the feature slice(s) prioritized by the team (default: core feature + necessary tests). (See Plan_v2 for ordering.)
- Increment 2 (WBS‑3.1): complete remaining scope, refactor, and finalize tests & docs.

Note: Tail Light service examples will be implemented as standalone example binaries only and will not change public library headers unless explicitly approved.

## Technical context
- Language / toolchain: C++17, CMake
- Networking / protocol focus: UDP transport and SOME/IP messaging
- Verification focus: unit tests, end-to-end tests, sanitizer-ready validation, build/test evidence capture

## Deliverables / Evidence
- MP1 charter (this document) — committed to docs/ai-sdlc/MP1_charter.md
- Implementation branch(es) and PR(s) with per‑hunk rationales
- Build logs and test logs for each PR (docs/ai-sdlc/evidence/build_<sha>.log, tests_<sha>.log)
- Coverage report and tests_AI_missed.md
- WBS handovers for WBS‑2.6 and WBS‑3.1
- MP1 completion report with measured productivity deltas (docs/ai-sdlc/evidence/MP1_completion_report.md)

## Success metrics
- Feature increments build and pass CI tests.
- Unit/integration tests added for new behavior; >=80% line coverage on changed files.
- Zero unexplained lines in PRs: every hunk has a one‑line rationale.
- Learning loops: at least 5 entries created; >=2 closures scheduled within the next increment.
- Productivity reporting: manual baseline vs actual hours recorded for MP1 tasks.

## Manual baseline estimate and measurement note
This charter was committed after AI-assisted WBS 2.2 through WBS 2.4 work had already begun. Therefore:
- The estimates for WBS 2.2 through WBS 2.4 are retrospective human baseline estimates.
- Productivity deltas for those earlier stages are useful for orientation only and are not a strict pre-AI baseline.
- Productivity deltas for implementation and later verification stages must be measured from explicit task-map entries and evidence captured from this point forward.

## Baseline manual estimate (recorded)
- WBS‑2.2 Requirement clarification & decomposition: 6 hours (retrospective estimate)
- WBS‑2.4 Planning & design (Plan v1 + challenge + Plan v2): 8 hours (retrospective estimate)
- WBS‑2.6 Increment 1 implementation (feature slice, tests, PR): 32 hours
- WBS‑2.7 Verification, coverage, and CI evidence: 10 hours
- WBS‑3.1 Increment 2 implementation and hardening: 32 hours
- WBS‑3.x Documentation, handovers, and completion reporting: 4 hours

Total manual baseline estimate: 92 hours

## Roles & Owners
- Product Owner / Requester: @eduycojr
- AI Operator: @copilot
- Implementer: @eduycojr
- Technical Lead / Architect: @eduycojr
- Reviewer / Approver: @eduycojr
- Test / Security Owner: @eduycojr
- Repository / CI Owner: @eduycojr

## Acceptance criteria (must be evidenced)
1. All PRs compile and pass tests in CI; build/test logs attached to evidence.
2. Tests added for new behavior; coverage report shows >=80% line coverage for changed code.
3. PR descriptions include per‑hunk one‑line rationales for every change.
4. Human reviewers approve PRs; review comments resolved and recorded.
5. MP1 completion report produced: baseline vs actual hours; defect counts; AI acceptance rate; learning‑loop entries.
6. Manual baseline measurement limitation is explicitly documented, and forward-looking task estimates are recorded before subsequent implementation work proceeds.

## Schedule & Timeline (tentative)
- Week 0: WBS‑2.2 approved and WBS‑2.3 execution evidence gathered (complete)
- Week 1: Plan v1, challenge pass, Plan v2 (complete)
- Week 2: Increment 1 implementation, tests, PR(s), review (current)
- Week 3: Increment 2, finalization, MP1 completion report, demo

## Next steps (to continue MP1)
1. Approve this MP1 charter (sign below).
2. Populate mp1_task_map.yaml with implementation and verification task estimates and observed actuals.
3. Start or continue feature branch work for WBS‑2.6 implementation and follow the Plan_v2 task ordering.
4. Keep handover artifacts current and attach build/test evidence to PRs.

---

### Approval (human)
- Approver: @eduycojr
- Approval Date (UTC): 2026-10-18T00:00:00Z
- Decision: APPROVE
- Comments: Approved with documented limitation that early-stage baseline estimates are retrospective because AI-assisted work had already begun before this charter was recorded.
