# WBS 2.1 Stage 10 — Verification and evidence mapping

**Purpose:** Map each acceptance criterion to actual executed command/CI evidence tied to the exact revision. Check that logs match the proposed/reviewed diff; separate suggested commands from executed results. Identify missing evidence.

**Relevant library prompts:** P15; P14 for structured schemas; P16 only for controlled before/after prompt experiments

## Inputs to provide
- WBS/task ID and approved scope
- Relevant requirement/acceptance criteria
- Upstream handover record and source artifact paths
- Repository branch/commit SHA and relevant file evidence
- Applicable constraints, tests, logs or policy (as required by this stage)

Replace task-specific details before submitting. If a required input is missing, use `UNKNOWN`/`BLOCKED` rather than inventing it.

## Copy/paste prompt

```text
Execute WBS 2.1 Stage 10: Verification and evidence mapping.
Task/WBS: [fill in]
Repository and revision: [fill in; UNKNOWN if not provided]
Requirement/scope: [fill in]
Prior-stage handover: [paste verified handover]
Relevant artifacts/evidence: [paths, excerpts, command output, issue; or UNKNOWN]
Constraints/non-goals: [fill in]

Use the relevant specialist prompt(s): P15; P14 for structured schemas; P16 only for controlled before/after prompt experiments. First state whether their files are accessible. If not, ask me to paste the required template; do not infer its contents.

Return: (1) stage and prompt IDs, (2) inputs/evidence inspected, (3) facts/inferences/unknowns, (4) result, (5) acceptance/evidence mapping, (6) verification/human action required, (7) unresolved risks, (8) status and concise next-stage handover. Never claim execution or approval without evidence.

Stage-specific instruction: Map each acceptance criterion to actual executed command/CI evidence tied to the exact revision. Check that logs match the proposed/reviewed diff; separate suggested commands from executed results. Identify missing evidence.

Stop condition: Stop if any required check lacks actual evidence, fails, or uses a different revision.
```

## Expected output
Verification matrix: criterion, command/CI job, observed result, evidence link, commit SHA, exception.

## Human exit check
- [ ] Inputs meet the selected prompt's contract.
- [ ] Facts cite evidence; assumptions/unknowns are explicit.
- [ ] Stage-specific deliverable is present.
- [ ] Required independent verification/human decision is recorded.
- [ ] Handover identifies next stage and open items.
- [ ] Stop condition is satisfied or status remains `BLOCKED`/`WAITING_FOR_INPUT`.
