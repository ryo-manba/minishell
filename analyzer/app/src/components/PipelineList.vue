<template lang="pug">

.pipelinelist
  .list(v-if="flattened_pipelinelist")
    .pipeline(
      v-for="pipeline in flattened_pipelinelist.pipelines"
    )
      .head
        h5 Pipeline
      .clause(
        v-for="clause in pipeline.clauses"
      )
        h5 Clause
        h5 Redirections
        .redirs(
          v-if="clause.redirs && clause.redirs.length > 0"
        )
          .redir(
            v-for="redir in clause.redirs"
          )
            .operator
              | {{ get_desc_redir_op(redir) }}
            | fd:
            code {{ get_redir_fd(redir).fd }}
            span(
              v-if="get_redir_fd(redir).default"
            ) (default)
            |, target:
            code(
              v-if="redir.operand_right"
            ) {{ redir.operand_right.token }}
        h5 Command Tokens
        .strees
          .stree(
              v-for="stree in clause.strees"
          )
            .token_id {{ stree.token_id }}
            .body
              pipeline-list(
                v-if="ss = reflat_subshell(stree)"
                :flattened_pipelinelist="ss"
              )
              code(
                v-else
              ) {{ stree.token }}
      .joint(v-if="pipeline.pipeline.joint")
        code {{ pipeline.pipeline.joint }}
</template>


<script lang="ts">
import _ from "lodash";
import { reactive, ref, Ref, SetupContext, defineComponent, onMounted, PropType, watch, computed } from '@vue/composition-api';
import * as Lexer from "@/models/lexer";
import * as Parser from "@/models/parser";
import * as MS from "@/models/minishell";
import * as EV from "@/models/envvar";

export default defineComponent({
  name: "pipeline-list",
  props: {
    flattened_pipelinelist: {
      type: Object,
      required: true,
    },
  },

  setup(prop: {
    flattened_pipelinelist: Parser.FlatPipelineList;
  }, context: SetupContext) {

    const reflat_subshell = (stree: MS.STree) => {
      const subshell = stree.subshell;
      if (!subshell) { return null; }
      return Parser.flatten_pipelinelist(subshell);
    };

    /**
     * リダイレクション演算子の説明
     */
    const get_desc_redir_op = (redir: MS.RedirList) => {
      switch (redir.op) {
        case "<": return "INPUT_FROM_FILE";
        case ">": return "OUTPUT_TO_FILE";
        case "<>": return "IN_AND_OUT_FILE";
        case ">>": return "APPEND_TO_FILE";
        case "<&": return "DUP_FOR_INPUT";
        case ">&": return "DUP_FOR_OUTPUT";
        case "<<": return "HEREDOC";
        case "<<-": return "HEREDOC_EXTAB";
      }
      return "* UNEXPECTED *"
    }
    const get_redir_fd = (redir: MS.RedirList) => {
      if (redir.operand_left) {
        return {
          fd: redir.operand_left.token,
          default: false,
        };
      }
      switch (redir.op) {
        case "<":
        case "<<":
        case "<<-":
        case "<&":
        case "<<":
          return {
            fd: "0",
            default: true,
          }
        default:
          return {
            fd: "1",
            default: true,
          }
      }
    }


    return {
      reflat_subshell,
      get_desc_redir_op,
      get_redir_fd,
    }
  },

})
</script>

<style lang="stylus" scoped>
.pipelinelist
  display flex
.list
  padding 4px
  border 1px solid blue
  display flex
  overflow-x scroll
  .pipeline
    padding 4px
    border 1px solid violet
    display flex
    justify-content center
    align-items center
    margin-left 4px
    .joint
      padding 4px
    .clause
      padding 4px
      border 1px solid green
      margin-left 4px
      .redirs, .strees
        padding 4px
        display flex
      .redir
        padding 4px
        border 1px solid black
        border-radius 8px
        margin-left 4px
      .stree
        padding 4px
        border 1px solid orange
        margin-left 4px
code
  white-space pre-wrap
  font-family "Courier New", Consolas, monospace
  font-weight bold
</style>