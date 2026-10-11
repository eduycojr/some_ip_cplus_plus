# WBS 2.1 Stage 04 — Implementation plan

**Purpose:** Create the smallest sequenced plan from the approved requirement, task tree, repo findings, constraints, and existing tests/toolchain. Include output, dependency, verification, rollback, and done condition per step. Reconcile every supplied constraint.

**Relevant library prompts:** P05; P04 only for real design alternatives; P17 for multi-turn gate

## Inputs to provide
- WBS/task ID and approved scope
- Relevant requirement/acceptance criteria
- Upstream handover record and source artifact paths
- Repository branch/commit SHA and relevant file evidence
- Applicable constraints, tests, logs or policy (as required by this stage)

Replace task-specific details before submitting. If a required input is missing, use `UNKNOWN`/`BLOCKED` rather than inventing it.

## Copy/paste prompt

```text
Execute WBS 2.1 Stage 4: Implementation plan.
Task/WBS: [fill in]
Repository and revision: [fill in; UNKNOWN if not provided]
Requirement/scope: [fill in]
Prior-stage handover: [paste verified handover]
Relevant artifacts/evidence: [paths, excerpts, command output, issue; or UNKNOWN]
Constraints/non-goals: [fill in]

Use the relevant specialist prompt(s): P05; P04 only for real design alternatives; P17 for multi-turn gate. First state whether their files are accessible. If not, ask me to paste the required template; do not infer its contents.

Return: (1) stage and prompt IDs, (2) inputs/evidence inspected, (3) facts/inferences/unknowns, (4) result, (5) acceptance/evidence mapping, (6) verification/human action required, (7) unresolved risks, (8) status and concise next-stage handover. Never claim execution or approval without evidence.

Stage-specific instruction: Create the smallest sequenced plan from the approved requirement, task tree, repo findings, constraints, and existing tests/toolchain. Include output, dependency, verification, rollback, and done condition per step. Reconcile every supplied constraint.

Stop condition: Stop for missing required toolchain/context or conflicts among constraints.
```

## Expected output
Plan with step-to-acceptance mapping and explicit assumptions.

## Human exit check
- [ ] Inputs meet the selected prompt's contract.
- [ ] Facts cite evidence; assumptions/unknowns are explicit.
- [ ] Stage-specific deliverable is present.
- [ ] Required independent verification/human decision is recorded.
- [ ] Handover identifies next stage and open items.
- [ ] Stop condition is satisfied or status remains `BLOCKED`/`WAITING_FOR_INPUT`.
