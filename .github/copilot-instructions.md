<!-- BEGIN WBS-2.1 AI-SDLC WORKFLOW ADAPTER -->
## AI-assisted SDLC workflow

When working in this repository, use `docs/ai-sdlc/WORKFLOW_MODEL_v1.md` as the canonical process and `docs/ai-sdlc/PROMPT_ROUTER.md` to choose the smallest applicable prompt set. The 11 stages, in order, are: (1) Requirement — P01; (2) Decomposition — P02; (3) Repository exploration — P03/P15; (4) Plan — P05, with P04/P17 only when useful; (5) Adversarial plan critique — `docs/ai-sdlc/stage-prompts/05_plan-critique.md`; (6) Implement — P06; (7) Test generation — P08; (8) Debug — P07/P09; (9) Human review — P10 and P11 when relevant; (10) Verify — actual execution evidence plus P14/P15 when relevant; (11) Document/maintain — P12/P13. P16 is a cross-stage evaluator, not a workflow stage; P18 applies to structured tool payloads when required.

The detailed WBS 1.4 prompt templates are in `docs/ai-sdlc/prompt-library/prompts/`. On GitHub.com Copilot Chat, these Markdown files are reference material; do not assume they are automatically invoked as IDE prompt files. Ask for or use the relevant prompt by path and copy its template into the conversation when retrieval is uncertain.

### Non-negotiable operating rules
- Treat repository context as evidence to inspect, not proof that every file or dependency has been examined. Cite repository paths, symbols, and excerpts for material claims.
- Treat every AI suggestion as a candidate. Do not invent files, APIs, requirements, commands, test results, build/CI outcomes, commits, pushes, pull requests, or approvals.
- Distinguish **FACT / INFERRED / UNKNOWN / PROPOSED**. Ask for missing required inputs or return `BLOCKED`; do not silently fill critical gaps with assumptions.
- Never claim a command, build, test, sanitizer, static analysis, CI job, or runtime behavior passed unless actual output from that execution is available in this conversation. Suggested commands are not executed commands.
- Do not make code changes, initiate an agent task, commit, push, merge, or create/update a pull request unless the user explicitly asks and the current interface provides the required capability. Otherwise return a reviewable patch/instructions.
- Stop for human clarification/approval when acceptance criteria, policy authorization, scope, externally visible behavior, security impact, destructive action, or a material plan risk is unresolved.
- Preserve existing architecture and APIs unless the approved requirement explicitly authorizes change. Prefer the smallest reversible change and include regression coverage.
- At each stage state: stage number/name; prompt ID(s) used; inputs/evidence inspected; output; assumptions/unknowns; verification needed; stop/exit decision; and a concise handover for the next stage.
- Carry forward approved requirements, constraints, file references, test results, unresolved items, and commit SHA explicitly. Do not assume a new chat or model has prior chat context.
- Apply security/data-handling rules from `docs/ai-sdlc/prompt-library/prompts/11_security_safe_use_review.md` and any actual approved policy supplied by the user. A training draft is not approved company policy.
- Use the 11 stages in the canonical workflow in order. A stage may be `N/A` only with a task-specific reason. Conditional specialist prompts are not mandatory on every task.
- Log an AI/process failure as a learning loop only when observed behavior, evidence, plausible cause, method change, retest, measure, and reuse status are distinguishable. Keep untested loops open.

### Free-tier / GitHub web operating mode
Use a single task-focused GitHub.com Copilot Chat conversation where practical. Bundle adjacent low-risk analysis stages to conserve the plan's limited usage, but retain explicit stage headings and gates. Ask for only the files needed; reuse a compact handover instead of repeating the entire repository. Repository custom instructions do not make the workflow deterministic and do not themselves run commands or tests. For verification, use actual GitHub Actions logs or a real local execution and paste the relevant output.
<!-- END WBS-2.1 AI-SDLC WORKFLOW ADAPTER -->
