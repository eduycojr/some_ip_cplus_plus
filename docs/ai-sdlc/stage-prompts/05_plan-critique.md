# WBS 2.1 Stage 05 — Adversarial plan critique (WBS 2.1 adapter prompt)

**Purpose:** Do not implement. Adversarially challenge the draft plan against requirements and repository evidence: hidden assumptions; missing failure/boundary cases; API/backward compatibility; concurrency/state; data/security/privacy/licensing; performance/resources; test quality; observability; rollback; migration; scope creep; dependency and CI gaps. For each finding cite the plan step/criterion/evidence, severity, likelihood/impact, and a concrete disposition. Identify what cannot be assessed from available evidence.

**Relevant library prompts:** P05 as source plan; optionally P10/P11 lenses

## Inputs to provide
- WBS/task ID and approved scope
- Relevant requirement/acceptance criteria
- Upstream handover record and source artifact paths
- Repository branch/commit SHA and relevant file evidence
- Applicable constraints, tests, logs or policy (as required by this stage)

Replace task-specific details before submitting. If a required input is missing, use `UNKNOWN`/`BLOCKED` rather than inventing it.

## Copy/paste prompt

```text
Execute WBS 2.1 Stage 5: Adversarial plan critique (WBS 2.1 adapter prompt).
Task/WBS: [fill in]
Repository and revision: [fill in; UNKNOWN if not provided]
Requirement/scope: [fill in]
Prior-stage handover: [paste verified handover]
Relevant artifacts/evidence: [paths, excerpts, command output, issue; or UNKNOWN]
Constraints/non-goals: [fill in]

Use the relevant specialist prompt(s): P05 as source plan; optionally P10/P11 lenses. First state whether their files are accessible. If not, ask me to paste the required template; do not infer its contents.

Return: (1) stage and prompt IDs, (2) inputs/evidence inspected, (3) facts/inferences/unknowns, (4) result, (5) acceptance/evidence mapping, (6) verification/human action required, (7) unresolved risks, (8) status and concise next-stage handover. Never claim execution or approval without evidence.

Stage-specific instruction: Do not implement. Adversarially challenge the draft plan against requirements and repository evidence: hidden assumptions; missing failure/boundary cases; API/backward compatibility; concurrency/state; data/security/privacy/licensing; performance/resources; test quality; observability; rollback; migration; scope creep; dependency and CI gaps. For each finding cite the plan step/criterion/evidence, severity, likelihood/impact, and a concrete disposition. Identify what cannot be assessed from available evidence.

Stop condition: Stop until critical/high findings are fixed or explicitly accepted by authorized human; request explicit human approval before implementation.
```

## Expected output
Critique table, accepted/rejected findings with reasons, revised plan, residual risks.

## Human exit check
- [ ] Inputs meet the selected prompt's contract.
- [ ] Facts cite evidence; assumptions/unknowns are explicit.
- [ ] Stage-specific deliverable is present.
- [ ] Required independent verification/human decision is recorded.
- [ ] Handover identifies next stage and open items.
- [ ] Stop condition is satisfied or status remains `BLOCKED`/`WAITING_FOR_INPUT`.
