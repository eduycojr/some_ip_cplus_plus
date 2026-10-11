# Prompt Router — WBS 2.1 × WBS 1.4 Prompt Library

Use this file to select prompts by **WBS 2.1 stage**. Do not treat the library's original working-stage numbers as the WBS 2.1 stage numbers: the taxonomies differ.

| WBS 2.1 stage | Primary prompt(s) | Optional prompt(s) | Notes |
|---|---|---|---|
| 1 Requirement | P01 | P15 | P15 checks a specific factual/technical claim, not requirements approval. |
| 2 Decomposition | P02 | P17 | P17 only if ambiguity/dependencies justify multi-turn planning. |
| 3 Repo exploration | P03 | P15 | Verify repository facts against actual files and cite paths/symbols. |
| 4 Plan | P05 | P04, P17, P08 | P04 only when real design alternatives exist; P08 helps plan test scope. |
| 5 Plan critique | Adapter-only `stage-prompts/05_plan_critique.md` | P10, P11 | P10/P11 are focused lenses, not a full plan-critique replacement. |
| 6 Implement | P06 | P18 | Only provide exact source/context; treat patch as proposal until inspected/applied. |
| 7 Test generation | P08 | P14 | P14 applies only when generated test/config/tool output must conform to a schema. |
| 8 Debug | P07 and/or P09 | P15 | P07 develops falsifiable hypotheses; P09 analyzes actual failure outputs. |
| 9 Human review | P10 | P11, P13 | Independent human decision remains required. |
| 10 Verify | P15 | P14, P16 | P16 only for comparable before/after prompt experiment output; none of these performs a real build by itself. |
| 11 Document/maintain | P12 | P13, P16 | P13 for refactoring/change control; P16 for measured prompt-improvement analysis. |

## The 18 prompts stay specialist components

- P01–P15 and P17–P18 are specialist task prompts in `prompt-library/prompts/`.
- P16 is a cross-stage experiment evaluator; it is not one of the 11 WBS 2.1 stages.
- `stage-prompts/05_plan_critique.md` is an adapter-stage playbook added because the source library does not include a dedicated plan-adversarial-review prompt.

## GitHub.com Copilot Free invocation

Repository instructions in `.github/copilot-instructions.md` are the automatic baseline. The other Markdown prompt templates are **not auto-run** by their presence in the repository. In GitHub.com Copilot Chat:

1. Select/open the intended repository context.
2. Ask Copilot to consult a specific path (for example `docs/ai-sdlc/prompt-library/prompts/01_requirements_clarification.md`).
3. If it cannot quote or accurately summarize that file's input contract, open the file yourself and paste its `Prompt template` section into chat.
4. Populate placeholders with actual, safe inputs; retain required stop conditions.
5. Save the answer and the independent verification evidence separately.
