# WBS 2.1 Stage 06 — Minimal scoped implementation

**Purpose:** Use the approved plan and actual code excerpts/paths. Propose the smallest patch matching acceptance criteria and conventions. Explain each changed section, preserved behavior, risks, and rollback. Do not broaden scope or invent signatures/APIs.

**Relevant library prompts:** P06; P18 only if exact schema/tool payload needed

## Inputs to provide
- WBS/task ID and approved scope
- Relevant requirement/acceptance criteria
- Upstream handover record and source artifact paths
- Repository branch/commit SHA and relevant file evidence
- Applicable constraints, tests, logs or policy (as required by this stage)

Replace task-specific details before submitting. If a required input is missing, use `UNKNOWN`/`BLOCKED` rather than inventing it.

## Copy/paste prompt

```text
Execute WBS 2.1 Stage 6: Minimal scoped implementation.
Task/WBS: [fill in]
Repository and revision: [fill in; UNKNOWN if not provided]
Requirement/scope: [fill in]
Prior-stage handover: [paste verified handover]
Relevant artifacts/evidence: [paths, excerpts, command output, issue; or UNKNOWN]
Constraints/non-goals: [fill in]

Use the relevant specialist prompt(s): P06; P18 only if exact schema/tool payload needed. First state whether their files are accessible. If not, ask me to paste the required template; do not infer its contents.

Return: (1) stage and prompt IDs, (2) inputs/evidence inspected, (3) facts/inferences/unknowns, (4) result, (5) acceptance/evidence mapping, (6) verification/human action required, (7) unresolved risks, (8) status and concise next-stage handover. Never claim execution or approval without evidence.

Stage-specific instruction: Use the approved plan and actual code excerpts/paths. Propose the smallest patch matching acceptance criteria and conventions. Explain each changed section, preserved behavior, risks, and rollback. Do not broaden scope or invent signatures/APIs.

Stop condition: Stop if code evidence conflicts with plan or change cannot be scoped/explained.
```

## Expected output
Reviewable diff/patch proposal and per-hunk rationale; clearly distinguish proposed from applied.

## Human exit check
- [ ] Inputs meet the selected prompt's contract.
- [ ] Facts cite evidence; assumptions/unknowns are explicit.
- [ ] Stage-specific deliverable is present.
- [ ] Required independent verification/human decision is recorded.
- [ ] Handover identifies next stage and open items.
- [ ] Stop condition is satisfied or status remains `BLOCKED`/`WAITING_FOR_INPUT`.
