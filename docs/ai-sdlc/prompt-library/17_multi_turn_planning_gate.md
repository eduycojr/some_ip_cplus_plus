# P17 — Multi-turn Planning — Use Only When Justified

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P17
- **Version:** 1.0.0
- **SDLC stage:** 4 — Planning
- **Derived from WBS 1.3 pair(s):** 11
- **Learning Loop tag:** W1.3-LL-01

## Purpose
Apply the WBS 1.3 learning-loop change: multi-turn planning is conditional, and any multi-turn result must reconcile all supplied constraints.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the task, requirements, dependencies, constraints, current plan, and unresolved questions. If there is no meaningful ambiguity/dependency density, prefer one-pass planning.

## Prompt template
```text
Decide whether multi-turn planning is justified for this engineering task.
Task: {{TASK}}
Requirements and constraints: {{REQUIREMENTS_CONSTRAINTS}}
Dependencies: {{DEPENDENCIES}}
Unresolved questions: {{UNKNOWNS}}

Choose ONE-PASS or MULTI-TURN and explain the expected benefit and interaction overhead. If MULTI-TURN, define what each turn resolves and stop once uncertainty is reduced enough. Before final output, perform a constraint reconciliation: every supplied requirement/constraint must be mapped to a plan step or explicitly marked unresolved. Do not silently omit compatibility or test constraints.
```

## Example output
Decision: ONE-PASS. The task is bounded and supplied constraints are clear; another turn would add overhead without expected completeness gain.

## Known limitation
A planning strategy choice remains a hypothesis until measured across comparable tasks. This does not close LL-01 until retest, measurement, and reuse are evidenced.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
