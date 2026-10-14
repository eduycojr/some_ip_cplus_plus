# WBS-3.3 — AI-assisted code review checklist

Repository: eduycojr/some_ip_cplus_plus
Branch: ai-assisted-sdlc
Author: GitHub Copilot (AI-assisted checklist, draft)

Purpose
- A reusable human code-review checklist tuned for AI-generated code risks (hallucinated APIs, wrong error handling, silent behaviour change, over-engineering, license risk). Use this as the canonical reviewer playbook for Stage 9 (Human review) of the AI-assisted SDLC.

Prompt & workflow references
- Workflow model: docs/ai-sdlc/WORKFLOW_MODEL_v1.md
- Prompt router: docs/ai-sdlc/PROMPT_ROUTER.md
- Primary review prompt: docs/ai-sdlc/prompt-library/10_code_review.md (P10)

How to use
1) Gather the exact diff or commit range to review (supply the Diff/Commit SHA in the review ticket or reviewer comment).
2) For each checklist item: record evidence (file path + line numbers or quoted snippet), severity, impact, trigger condition, suggested fix, and suggested test.
3) Label claims as FACT / INFERRED / UNKNOWN / PROPOSED.
4) Do not merge or accept changes until high-severity items are addressed and verified by tests or sanitizers (ASAN/TSAN/UBSAN) as applicable.

Checklist (grouped by risk)

High risk — correctness & runtime safety (Must pass)
- H1: Hallucinated API / ABI check
  - Verify every referenced symbol exists in the repository or declared dependency header; match signatures exactly to headers in include/. If not present, mark as HIGH and BLOCK.

- H2: Error-handling and propagation
  - Confirm failures return documented error codes or throw documented exceptions; no silent swallow of errors in server or transport paths.

- H3: Silent behaviour change / contract violations
  - Compare the change against REQUIREMENTS.md or public API docs; queries must not mutate state unless explicit in requirements.

- H4: Protocol framing and wire-format correctness
  - Validate header and length fields; ensure serialize/deserialize round-trip is preserved; mismatched constants (e.g., SIZE vs MIN_LENGTH) are HIGH severity.

- H5: Concurrency and thread-safety
  - Check shared mutable state guarded by mutex/atomics; ensure callbacks are copied under lock and invoked outside; require TSAN runs for suspicion.

- H6: Resource lifetime / cleanup
  - Verify sockets, file descriptors, threads are closed/joined on stop/error; no use-after-close risk.

Medium risk — correctness/maintainability (Must consider)
- M1: Retry/backoff semantics & bounded failures
  - Retries must be bounded and respect configured timeouts; verify logs show retries and final failure is surfaced to caller.

- M2: Input validation & malformed input handling
  - Deserializers must validate lengths and ranges and not crash on malformed packets.

- M3: Observability & logging adequacy
  - Ensure logs contain sufficient context (service/method/client/session ids) and failures are logged with reasons.

- M4: Test coverage & regression tests
  - Verify new behavior has unit/integration tests covering success, failure, and edge cases; require tests be added for each bug fix.

- M5: Over-engineering & scope creep
  - Flag non-required abstractions or broad API changes not justified by the requirement; prefer the smallest reversible change.

Low risk — style & documentation
- L1: API documentation / header comments
  - Public functions that change behavior must have updated doc comments and release notes.

- L2: Naming / code style
  - Follow repository conventions; small style issues are low severity but should be recorded.

Legal / licence risk (Must verify)
- C1: Licence provenance and copied code detection
  - Any inserted code block from outside must include attribution and a compatible license; run license scan for suspicious snippets.

- C2: New dependencies and license compatibility
  - New third-party libs must be documented and license-checked for compatibility with project constraints.

Security & privacy
- S1: Memory safety and sanitizers
  - Use ASan/UBSan/TSan for suspect changes; failing sanitizers require HIGH severity action.

- S2: Input validation affecting security (parsing, buffer ops)
  - Ensure deserializers check maximum lengths and do not memcpy past buffer bounds.

Operational / CI
- O1: CI and build matrix
  - Ensure change does not bypass CI checks; ensure changes build on target platforms in CMake.

- O2: Release and handover artifacts
  - PRs must include per-hunk one-line rationales (MP1 acceptance requirement). Attach build/test logs to handover.

Evidence and reporting template (per finding)
- Finding ID: (e.g., H1-01)
- Severity: High / Medium / Low
- Claim label: FACT / INFERRED / UNKNOWN / PROPOSED
- Evidence: file path(s) + line numbers or quoted snippet
- Impact: short description
- Trigger condition: how to reproduce
- Suggested fix: code-level action (hunk-level suggestion)
- Suggested verification: test name / sanitizer / CI step

Minimum acceptance for WBS-3.3 deliverable
- Checklist file present in repo under docs/ai-sdlc/checklists/
- Checklist contains >= 15 concrete items grouped by risk (this file meets that requirement)
- Checklist applied to MP1 and one peer PR or commit; review findings recorded as issues or review comments and linked in handover.
- At least 3 real issues identified and recorded with evidence (I applied this checklist to commit 71070856... and produced 3 findings recorded in a separate review artifact.)

Notes
- This checklist is a living artifact. Keep a short changelog when modifying it and reference the P10 prompt used to draft it.

