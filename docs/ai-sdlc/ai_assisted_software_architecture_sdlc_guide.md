# AI-Assisted Software Architecture SDLC — Operating Guide (Final, Audit-Aligned)
Comprehensive, tool‑neutral operating model for using AI assistants across the software development lifecycle with emphasis on architecture work, reliable context transfer, artifact traceability, and evidence‑based stage gates.

## Purpose
- Define an artifact-driven, stage-gated workflow for AI-assisted architecture & engineering work.
- Ensure reproducible context reconstruction across AI sessions, persistent handovers, and evidence-first acceptance decisions.
- Provide manifest and handover schemas, telemetry mapping, learning-loop registers, and explicit evidence rules so reviewers can verify claims before acceptance.

## Audience
- Software architects, technical leads, developers, testers, reviewers, and learners using GitHub Copilot, Claude, Gemini, or other assistants. Tool capabilities vary; verify actual interface behavior before assuming automation.

## Core principle
- Artifacts are the source of truth; AI outputs are proposals until an authorized human attaches verifiable evidence that satisfies stage exit criteria.

## Executive summary
- Use an artifact-first, stage-gated process. Persist requirements, ADRs, handovers, manifests, telemetry mapping, tests, and evidence outside chat in canonical repo locations.
- Enforce Evidence-over-Assertion: do not mark criteria PASS without attached, verifiable artifacts (commit SHAs, CI artifact URLs, or signed reviews).
- Maintain a manifest (docs/ai-sdlc/manifest.yaml) that indexes canonical artifacts and their revision/evidence statuses.
- Require an explicit Context Receipt at session start and a Closure Handover at session end.
- Maintain a learning-loop register to capture AI/Process failures and scheduled remediations.

---

### 1. Principles (operational)
- Artifacts over chat: preserve authoritative artifacts in the repo or approved store.
- Single source of truth: each approved artifact has one canonical file; other references link to it.
- Selective context: supply minimal sufficient context for the task, using file paths and commit SHAs.
- Evidence-first: record exact commands, environments, and outputs for verification.
- Explicit uncertainty: label items FACT / INFERRED / PROPOSED / UNKNOWN.
- Human accountability: humans approve acceptance and consequential changes.
- Least privilege: do not include secrets in prompts or repo artifacts.
- Tool neutrality: templates and manifests work across providers.

### 2. Roles & responsibilities
- Requester/Product Owner: clarifies intent and acceptance.
- AI Operator: loads context, bounds prompts, runs authorized checks if available, records evidence.
- AI Assistant: produces analysis, drafts, and proposals, labels uncertainty.
- Architect/Tech Lead: owns architectural decisions.
- Reviewer/Approver: validates evidence and approves deliverables.
- Test/Security Owner: defines acceptance tests and reviews risk/controls.
- Repo/CI Owner: ensures reproducible builds and artifact retention.

### 3. Reference architecture (layers)
- Global workflow (lifecycle, gates, statuses).
- Project-local instructions (.github/copilot-instructions.md).
- Tool adapters (behavior notes for each AI provider).
- Session contracts (startup, closure, recovery).
- Artifact/evidence store (docs/ai-sdlc/evidence/, CI artifacts).
- Governance/tracking (manifest, telemetry mapping, WBS status).

### 4. Canonical folder layout (recommended)
project/
├── README.md
├── .github/
│   └── copilot-instructions.md
├── docs/
│   └── ai-sdlc/
│       ├── manifest.yaml
│       ├── telemetry/
│       │   └── mp1_task_map.yaml
│       ├── handovers/
│       ├── evidence/
│       ├── templates/
│       │   ├── session-startup.md
│       │   └── session-closure.md
│       ├── prompt-library/
│       └── stage-prompts/

---

### 5. Manifest (docs/ai-sdlc/manifest.yaml)
- Purpose: index canonical artifacts, their commits, evidence, status, and approvers.
- Rules:
  - Update manifest in same commit that produces/approves the artifact, when feasible.
  - Each entry includes id, path, commit SHA(s), status, approver (if any), and evidence links.
- Example:
```yaml
manifest_version: 1
project: some_ip_cplus_plus
last_updated: 2026-10-13T00:00:00Z
artifacts:
  - id: wbs_2_7_handover
    path: docs/ai-sdlc/handovers/WBS-2.7_to_WBS-2.8.md
    commit: c2475f19e1bea3a284d7c01dd0b16d755044f937
    status: closed_with_open_items
    approver: null
  - id: w2_telemetry
    path: docs/ai-sdlc/evidence/W2_telemetry_WBS-2.8.md
    commit: 14d3d6006639192320d4bc20ce3db48ffccf0fa8
    status: draft
learning_loops: docs/ai-sdlc/evidence/W2_learning_loops.md
telemetry:
  mp1_task_map: docs/ai-sdlc/telemetry/mp1_task_map.yaml
```

---

### 6. Handover record (mandatory fields)
- Every handover file must use the standard template (docs/ai-sdlc/handovers/WBS-x_to_WBS-y.md) and include:
  - Session ID, Repo/Branch, Inspected commit(s) (explicit SHA), Stage name, Prompt IDs used.
  - Inputs inspected: path + commit SHA.
  - Confirmed facts with evidence links.
  - Predecessor handover path + predecessor SHA (explicit upstream inheritance).
  - Inferences, assumptions, unknowns & blockers with owners.
  - Files changed / created with commit SHAs & diff links.
  - Commands executed + linked logs.
  - Acceptance criteria mapping (PASSED / FAILED / BLOCKED / NOT_RUN / N/A) with evidence.
  - Next-stage required inputs (exact paths + SHAs).
  - Status and Handover commit SHA.
- Enforcement: if any referenced file lacks a SHA, the handover must be BLOCKED until resolved.

---

### 7. Session Startup procedure (mandatory)
- Steps:
  1. Assign session ID S[YYYYMMDD]-[WBS]-[NN].
  2. Declare objective, scope, acceptance criteria, and outputs.
  3. Read workflow and repo instructions.
  4. Open previous handover(s) and cite predecessor path + SHA.
  5. Inspect required artifacts and record path + SHA for each.
  6. Produce a Context Receipt containing:
     - Objective & acceptance criteria
     - Files inspected and SHAs
     - Confirmed FACTs (with evidence)
     - Inferences & assumptions
     - Missing/blocked items with owners
     - Entry gate: PASS / BLOCKED (explain)
  7. If BLOCKED: stop; list required actions and owners. If PASS: proceed within declared scope.
- Startup prompt (templated): include required fields and require Context Receipt.

---

### 8. Work rules
- Always cite paths and commit SHAs for facts.
- Distinguish AI proposals from verified evidence.
- When proposing code, include a minimal reversible diff and tests to add.
- Do not claim tests/builds passed without attached logs/CI artifacts.
- For sanitizer/coverage claims, include runner environment and attach outputs.

---

### 9. Session Closure & Artifact Collection (mandatory)
- Steps:
  1. List files changed with commit SHAs (or explain why commit not performed).
  2. Attach build/test/sanitizer logs and CI artifacts (permitted evidence types).
  3. Map acceptance criteria to statuses with evidence links.
  4. Persist the handover file and update manifest.yaml in the same commit when possible.
  5. Publish closure status: Closed / Closed with open items / Blocked / Interrupted-recovery required.
  6. Assign next-stage required inputs (paths + SHAs) and owners for open items.
- Closure prompt (templated) enforces these outputs; closure cannot be marked VERIFIED without evidence.

---

### 10. Evidence rules (strict)
- Accepted evidence types:
  - Commit SHA(s) with diff link to the repository.
  - CI run ID and artifact URLs (download/permalink).
  - Structured logs (build/test/sanitizer) stored in docs/ai-sdlc/evidence/ or CI artifact store and referenced by URL.
  - Human approval: PR review with reviewer and timestamp, or signed entry in handover with approver identity.
- Minimal evidence requirements:
  - Test PASS: test runner log showing test name, PASS/FAIL, timestamp, and commit SHA.
  - Build PASS: CMake configure + build log with produced binary names and commit SHA.
  - Sanitizer/PenTest/Coverage: corresponding tool output attached.
- Any assertion lacking these attachments is UNVERIFIED and must be recorded as UNKNOWN or BLOCKED.

---

### 11. Telemetry & MP1 task mapping (required)
- Create docs/ai-sdlc/telemetry/mp1_task_map.yaml with:
```yaml
- mp1_task: WBS-2.6-IMPLEMENT
  pr: null
  commit_start: null
  commit_done: null
  session_id: S20261012-W2-01
  start_event: session_start
  done_event: handover_commit
  owner: '@owner'
```
- This mapping is mandatory for computing cycle-times and acceptance metrics. W2 telemetry files must reference this mapping.

---

### 12. Learning-loop register (docs/ai-sdlc/evidence/learning_loops.md)
- Required fields: id, title, description, evidence links, root-cause, proposed change, owner, scheduled retest, verification artifacts.
- Learning loops must be included in manifest.yaml and have owners and retest dates. Closure of a learning-loop requires evidence.

---

### 13. Chain-of-Custody Audit Matrix (template for handover & audit)
- For each handover session produce a small matrix and record PASS/FAIL for four criteria:
  1. Upstream Inheritance (predecessor path + SHA pinned)
  2. Artifact Persistence (evidence files attached and inventoried)
  3. Open Items & Learning Loops (learning loops present and scheduled)
  4. Context Completeness (Context Receipt present and required inputs verified)
- Auditor must attach audit evidence (file paths and SHAs) and set remedial actions for any FAIL.

---

### 14. Acceptance statuses & exit gate rules
- Use only: PASSED / FAILED / BLOCKED / NOT_RUN / NOT_APPLICABLE.
- Stage exit = PASSED only if:
  - All acceptance criteria mapped to evidence stored in docs/ai-sdlc/evidence/ or CI artifacts and listed in the handover.
  - No unresolved BLOCKED items that negate the decision.
- Otherwise stage = BLOCKED or CLOSED_WITH_OPEN_ITEMS. Owners and due dates required.

---

### 15. Recovery procedure (for interrupted or unclear sessions)
- Read manifest, predecessor handover, and last commit SHAs.
- Verify which claims are persisted (by SHA and evidence file presence).
- Produce a recovery handover listing verified items vs missing items.
- Do not continue past safety gates until missing items are resolved.

---

### 16. Tool adapter guidance (one per AI provider)
- For each AI provider record:
  - Which instruction files the tool loads automatically.
  - What file/attachment access is available.
  - How to persist files (commit API, copy/paste instructions).
  - How to capture outputs (logs, screenshots, artifact links).
  - Limitations and required manual follow-ups.
- Keep adapters short and in docs/ai-sdlc/adapters/.

---

### 17. Enforcement checklist & automation suggestions
- Manual checklist (required before stage exit):
  - [ ] Handover includes predecessor path + SHA.
  - [ ] All inputs referenced include commit SHAs.
  - [ ] Manifest.yaml updated or scheduled to be updated in same commit.
  - [ ] Evidence files attached or CI artifact links present.
  - [ ] mp1_task_map.yaml updated or scheduled.
  - [ ] Learning-loop entries referenced with owners.
  - [ ] Context Receipt present and entry gate is PASS.
- Automation recommendations (optional):
  - CI job that, on handover commit, verifies manifest entries and attached evidence file names, failing if required fields missing.
  - Pre-commit hook or GitHub Action that enforces manifest.yaml schema and handover template completion.
  - Telemetry job that computes cycle-times from mp1_task_map and stores metrics in docs/ai-sdlc/evidence/.

---

### 18. Metrics & telemetry (recommended)
- Handover completeness percentage.
- Artifact traceability score (artifacts with SHAs / required artifacts).
- Unverified claim count.
- Gate escape rate (work started with unmet entry gate).
- Learning-loop closure rate.
- Use metrics iteratively to improve the process.

---

### 19. Implementation rollout
1. Ratify workflow and assign owner.
2. Commit templates and manifest schema to docs/ai-sdlc/templates/.
3. Create initial manifest.yaml and mp1_task_map.yaml placeholder entries.
4. Pilot on one MP1 task and complete a startup → implementation → closure cycle with evidence.
5. Add CI checks to verify manifest/handover completeness.
6. Iterate and scale.

---

### 20. Definition of Done (Architecture & SDLC)
- Global workflow and templates exist and are version-controlled.
- Manifest and mp1_task_map are present and used.
- Each handover includes predecessor path + SHA and evidence attachments for PASS criteria.
- Learning loops recorded and scheduled.
- A fresh session can reconstruct context from artifacts without relying on chat memory.

---

Appendices
- Appendix A: Minimal handover example (complete with predecessor SHA and evidence links).
- Appendix B: Manifest schema example (YAML).
- Appendix C: mp1_task_map schema example (YAML).
- Appendix D: Chain-of-custody audit matrix template.
- Appendix E: Short reviewer checklist and common failure modes.

Closing note
- This guide formalizes the Evidence-over-Assertion principle and remediates the audit-identified gaps. Use the manifest and telemetry mapping to make SDLC telemetry computable and auditable. Require human approval on material decisions and evidence attachments for all PASSED claims.
