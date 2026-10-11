# WBS 2.1 Stage 03 — Repository exploration

**Purpose:** Explore the selected repository context using evidence. Identify entry points, relevant files/symbols, module boundaries, build/test configuration, data/control flow, and likely extension points. Cite paths and short evidence. Do not claim exhaustive inspection.

**Relevant library prompts:** P03; P15 for source-based claim checking

## Inputs to provide
- WBS/task ID and approved scope
- Relevant requirement/acceptance criteria
- Upstream handover record and source artifact paths
- Repository branch/commit SHA and relevant file evidence
- Applicable constraints, tests, logs or policy (as required by this stage)

Replace task-specific details before submitting. If a required input is missing, use `UNKNOWN`/`BLOCKED` rather than inventing it.

## Copy/paste prompt

```text
Execute WBS 2.1 Stage 3: Repository exploration.
Task/WBS: [fill in]
Repository and revision: [fill in; UNKNOWN if not provided]
Requirement/scope: [fill in]
Prior-stage handover: [paste verified handover]
Relevant artifacts/evidence: [paths, excerpts, command output, issue; or UNKNOWN]
Constraints/non-goals: [fill in]

Use the relevant specialist prompt(s): P03; P15 for source-based claim checking. First state whether their files are accessible. If not, ask me to paste the required template; do not infer its contents.

Return: (1) stage and prompt IDs, (2) inputs/evidence inspected, (3) facts/inferences/unknowns, (4) result, (5) acceptance/evidence mapping, (6) verification/human action required, (7) unresolved risks, (8) status and concise next-stage handover. Never claim execution or approval without evidence.

Stage-specific instruction: Explore the selected repository context using evidence. Identify entry points, relevant files/symbols, module boundaries, build/test configuration, data/control flow, and likely extension points. Cite paths and short evidence. Do not claim exhaustive inspection.

Stop condition: Stop before a patch plan if the relevant behavior or call path is not evidenced.
```

## Expected output
Evidence-based repo map plus inspected paths, test/build discovery, and unknowns.

## Human exit check
- [ ] Inputs meet the selected prompt's contract.
- [ ] Facts cite evidence; assumptions/unknowns are explicit.
- [ ] Stage-specific deliverable is present.
- [ ] Required independent verification/human decision is recorded.
- [ ] Handover identifies next stage and open items.
- [ ] Stop condition is satisfied or status remains `BLOCKED`/`WAITING_FOR_INPUT`.
