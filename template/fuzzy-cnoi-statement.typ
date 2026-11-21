// Typst template for making NOI-like problems
// Author: Wallbreaker5th
// MIT-0 License

#import "tablem.typ": tablem, three-line-table
#let 字号 = (
  初号: 42pt,
  小初: 36pt,
  一号: 26pt,
  小一: 24pt,
  二号: 22pt,
  小二: 18pt,
  三号: 16pt,
  小三: 15pt,
  四号: 14pt,
  中四: 13pt,
  小四: 12pt,
  五号: 10.5pt,
  小五: 9pt,
  六号: 7.5pt,
  小六: 6.5pt,
  七号: 5.5pt,
  小七: 5pt,
)

#let default-fonts = (
  // Typst will use the first installed family in each list.  This keeps the
  // original appearance when its fonts are available, while allowing the
  // template to compile cleanly on a standard Windows installation.
  mono: ("Fira Mono", "Consolas"),
  serif: "New Computer Modern",
  cjk-serif: ("FZShuSong-Z01S", "SimSun"),
  cjk-sans: ("FZHei-B01S", "SimHei"),
  cjk-mono: ("FZFangSong-Z02S", "Consolas"),
  cjk-italic: ("FZKai-Z03S", "SimSun"),
)

// Replace the three logo files in the matching folders with your own image.
// Set any path below to `none` to disable that logo. Widths can be
// adjusted from the document with `default-logo-settings + (...)`.
#let default-logo-settings = (
  watermark: "watermark-logo/watermark-faded.png",
  watermark-width: 100mm,
  header: "header-logo/logo.png",
  header-width: 30mm,
  first-page: "first-page-logo/logo.png",
  first-page-width: 25mm,
)


#let empty-par = {
  par()[
    #v(0em, weak: true)
    #h(0em)
  ]
}
#let add-empty-par(it) = {
  it
  empty-par
}


#let default-problem-fullname(problem) = {
  if (problem.at("name", default: none) != none) {
    problem.name
    if (problem.at("name-en", default: none) != none) {
      "（" + problem.name-en + "）"
    }
  } else {
    problem.at("name-en", default: "")
  }
}

#let default-header(contest-info, current-problem, logo: none) = {
  set par(first-line-indent: 0em)
  if (current-problem == none) {
    if logo != none {
      align(center, logo)
      v(-2pt)
      line(length: 100%, stroke: 0.3pt)
    }
    return
  }
  set text(size: 字号.五号)
  let round = contest-info.at("round", default: none)
  grid(
    columns: (1fr, auto, 1fr),
    align: (left, center, right),
    contest-info.name,
    if logo != none { logo } else { [] },
    [
      #if round != none { round + " " }
      #default-problem-fullname(current-problem)
    ],
  )
  v(-2pt)
  line(length: 100%, stroke: 0.3pt)
}

#let default-footer(contest-info, current-problem) = {
  if (current-problem == none) {
    return
  }
  set text(size: 字号.五号)
  align(center,{
    [第]
    counter(page).display()
    [页]
    h(2em)
    [共]
    context{
      let cnt = counter(page).final().at(0)
      link((page:cnt, x:0pt, y:0pt), text(fill: rgb("#00f"), [#cnt]))
    }
    [页]
  })
}

#let make-formula(s) = {
  let numbers = s.matches(regex("[\d.]+"))
  let last-end = 0
  for n in numbers {
    let start = n.start
    let end = n.end
    let number = n.text
    s.slice(last-end, start)
    math.equation(number)
    last-end = end
  }
  s.slice(last-end, s.len())
}

#let three-line-table = tablem.with(
  render: (columns: auto, align: center, ..args) => {
    table(
      columns: columns,
      stroke: none,
      align: center + horizon,
      table.hline(y: 0),
      table.hline(y: 1, stroke: .5pt),
      ..args,
      table.hline(),
    )
  }
)

#let data-constraints-table-args = {
    (
      stroke: (x, y) => (
        left: if (x > 0) { .4pt },
        bottom: 2pt,
        top: if(y == 0) { 2pt } else if (y == 1) { 1.2pt } else { .4pt }
      ),
      align: center+horizon,

    )
  }

#let current-problem-idx = counter("current-problem-idx")
#let statement-page-begin = state("statement-page-begin", false)

#let document-class(
  contest-info,
  problem-list,
  custom-fonts: (:),
  header: default-header,
  footer: default-footer,
  logo-settings: default-logo-settings,
) = {
  let fonts = default-fonts + custom-fonts
  // Turn a user-supplied font name or the default priority array into a
  // flat priority list accepted by Typst's `text.font` parameter.
  let font-list(font, fallback) = if type(font) == array {
    font
  } else {
    (font, fallback)
  }
  let mono-fonts = font-list(fonts.mono, "Consolas")
  let serif-fonts = font-list(fonts.serif, "SimSun")
  let cjk-serif-fonts = font-list(fonts.cjk-serif, "SimSun")
  let cjk-sans-fonts = font-list(fonts.cjk-sans, "SimHei")
  let cjk-mono-fonts = font-list(fonts.cjk-mono, "Consolas")
  let cjk-italic-fonts = font-list(fonts.cjk-italic, "SimSun")
  let logo(path, width) = if path == none { none } else { image(path, width: width) }
  let watermark-logo = logo(logo-settings.watermark, logo-settings.watermark-width)
  let header-logo = logo(logo-settings.header, logo-settings.header-width)
  let first-page-logo = logo(logo-settings.first-page, logo-settings.first-page-width)

  let get-current-problem(here) = {
    let idx = current-problem-idx.at(here).at(0) - 1
    if (idx >= 0) { problem-list.at(idx) }
  }

  let init(it) = {
    set text(font: ((
      name: serif-fonts.at(0),
      covers: "latin-in-cjk"
    ), ..cjk-serif-fonts, ..serif-fonts.slice(1)), size: 字号.小四, lang: "zh", region: "CN")
    show emph: set text(font: ((
      name: serif-fonts.at(0),
      covers: "latin-in-cjk"
    ), ..cjk-italic-fonts, ..serif-fonts.slice(1)))
    show strong: st => {  // Dotted strong text
      set text(font: ((
        name: serif-fonts.at(0),
        covers: "latin-in-cjk"
      ), ..cjk-sans-fonts, ..serif-fonts.slice(1)))
      show regex("\p{sc=Hani}+"): s => {
        underline(s, offset: 3pt, stroke: (
          cap: "round",
          thickness: 0.1em,
          dash: (array: (0em, 1em), phase: 0.5em)
        ))
      }
      st
    }
    show raw: set text(font: ((
      name: mono-fonts.at(0),
      covers: "latin-in-cjk"
    ), ..cjk-mono-fonts, ..mono-fonts.slice(1)), size: 字号.小四)

    set list(indent: 1.75em, marker: ([•], [#h(-1em)•]))
    set enum(
      indent: 1.75em,
      numbering: x => {
        grid(
          columns: (0em, auto),
          align: bottom,
          hide[悲], numbering("1.", x)
        ) // As a workaround
      }
    )

    show heading: set text(font: ((
      name: serif-fonts.at(0),
      covers: "latin-in-cjk"),
    ..cjk-sans-fonts, ..serif-fonts.slice(1)), weight: 500)
    show heading.where(level: 1): it => {
      set text(size: 字号.小二)
      set heading(bookmarked: true)
      pad(top: 8pt, align(center, it))
    }
    show heading.where(level: 2): it => {
      v(-8pt)
      set text(size: 字号.小四)
      set heading(bookmarked: true)
      pad(left: 1.5em, top: .76em, bottom: .36em, [【]+box(it)+[】])

    }
show raw.where(block: true): it => {
  show raw.line: line => {
    if (line.text == "" and line.number == line.count) {
      return []
    }
    box(
      grid(
        columns: (0em, 1fr),
        align: (right, left),
        move(
          text(str(line.number), fill: gray, size: 0.8em),
          dx: -1em,
          dy: 0.1em,
        ),
        line.body
      )
    )
  }
  block(
    width: 100%,
    inset: (left: 0.5em, right: 0.5em, top: 0.36em, bottom: 0.36em),
    stroke: 0.5pt + luma(120),
    breakable: true,
    {
      set par(first-line-indent: 0em)
      it
    }
  )
}
    // show raw.where(block: true): it => {
    //   show raw.line: it => {
    //     if (it.text == "" and it.number == it.count) {
    //       return
    //     }
    //     box(
    //       grid(
    //         columns: (0em, 1fr),
    //         align: (right, left),
    //         move(
    //           text(str(it.number), fill: gray, size: 0.8em),
    //           dx: -1em,
    //           dy: 0.1em,
    //         ),
    //         it.body
    //       )
    //     )
    //   }
    //   pad(
    //     rect(it, stroke: 0.5pt + rgb("#00f"), width: 100%, inset: (y: 0.7em)),
    //     left: 0.5em
    //   )
    // }

    // Page Layout
    set page(
      margin: 2.2cm,
      header: context if header-logo == none {
        header(contest-info, get-current-problem(here()))
      } else {
        default-header(contest-info, get-current-problem(here()), logo: header-logo)
      },
      footer: context if(statement-page-begin.at(here())) {
        footer(contest-info, get-current-problem(here()))
      },
      background: if watermark-logo != none {
        align(center + horizon, watermark-logo)
      },
      foreground: context if counter(page).get().at(0) == 1 and first-page-logo != none {
        align(center + bottom, first-page-logo)
      },
    )

    show table: pad.with(y: .5em)

    set par(first-line-indent: (amount: 2em, all: true), leading: 0.52em)
    // Looks right but I'm not sure about the exact value
    set par(spacing: 0.48em)

    it
  }


  let title() = {
    align(center, {
      text(contest-info.name, size: 字号.二号, font: ((
        name: serif-fonts.at(0),
        covers: "latin-in-cjk"
      ), ..cjk-sans-fonts, ..serif-fonts.slice(1)))

      parbreak()
      v(10pt)

      let name-en = contest-info.at("name-en", default: none)
      if (name-en != none) {
        text(name-en, size: 字号.小一)
        parbreak()
        v(10pt)
      }


      let round = contest-info.at("round", default: none)
      if (round != none) {
        emph(text(round, size: 字号.二号))
        parbreak()
      }

      let author = contest-info.at("author", default: none)
      if (author != none) {
        text(author, size: 字号.小三, font: ((
          name: serif-fonts.at(0),
          covers: "latin-in-cjk"
        ), ..cjk-sans-fonts, ..serif-fonts.slice(1)))
        parbreak()
        v(5pt)
      }

      let time = contest-info.at("time", default: none)
      if (time != none) {
        text(time, size: 字号.小三, font: ((
          name: serif-fonts.at(0),
          covers: "latin-in-cjk"
        ), ..cjk-sans-fonts, ..serif-fonts.slice(1)))
      }
    })
  }

  let problem-table(
    extra-rows: (:),
    languages: (("C++", "cpp"),),
    compile-options: (("C++", "-O2 -std=c++14 -static"),),
  ) = {
    let default-row = (
      wrap: text,
      always-display: false,
      default: "无",
    )
    let rows = (
      name: (
        name: "题目名称",
        always-display: true,
      ),
      type: (
        name: "题目类型",
        always-display: true,
        default: "传统型",
      ),
      name-en: (
        name: "目录",
        wrap: raw,
        always-display: true,
      ),
      executable: (
        name: "可执行文件名",
        wrap: raw,
        always-display: true,
        default: x => x.name-en,
      ),
      input: (
        name: "输入文件名",
        wrap: raw,
        always-display: true,
        default: x => x.name-en + ".in",
      ),
      output: (
        name: "输出文件名",
        wrap: raw,
        always-display: true,
        default: x => x.name-en + ".out",
      ),
      time-limit: (
        name: "每个测试点时限",
        wrap: make-formula,
      ),
      memory-limit: (
        name: "内存限制",
        wrap: make-formula,
      ),
      test-case-count: (
        name: "测试点数目",
        default: "10",
        wrap: make-formula,
      ),
      subtask-count: (
        name: "子任务数目",
        default: "1",
      ),
      test-case-equal: (
        name: "测试点是否等分",
        default: "是",
      ),
      pre-test-case-count: (
        name: "预测试点数目",
        default: "无",
        wrap: make-formula,
      ),
      author :(
        name: "出题人",
        default: p => ( p. author)
      )
    ) + extra-rows
    rows = rows.pairs().map(
      row => (row.at(0), default-row + row.at(1))
    )

    set table(align: bottom)
    let first-column-width = if (problem-list.len() <= 3) { 22% } else { 1fr }
    let columns = (first-column-width, ) + (1fr, ) * problem-list.len()
    table(
      columns: columns,
      stroke: 0.4pt,
      ..{
        rows.filter(row => {
          let (field, r) = row
          if (r.always-display) {
            true
          } else {
            problem-list.any(p => p.at(field, default: none) != none)
          }
        }).map(row => {
          let (field, r) = row
          (text(r.name), )
          problem-list.map(p => {
            let v = p.at(field, default: none)
            let w = if (v == none) {
              if (type(r.default) == str) {
                (r.wrap)(r.default)
              } else if (type(r.default) == function) {
                (r.wrap)((r.default)(p))
              } else if (type(r.default) == content) {
                (r.wrap)(r.default)
              }
            } else {
              v
            }

            if (type(w) == str) {
              (r.wrap)(w)
            } else {
              w
            }
          })
        }).flatten()
      }
    )

    [提交源程序文件名]
    table(
      columns: columns,
      stroke: 0.4pt,
      align: horizon,
      ..{
        languages.map(l => {
          if (l.len() == 2) {
            l += (1em, )
          }
          (text(size: l.at(2))[对于 #l.at(0) #h(1fr) 语言], )
          problem-list.map(p => {
            let v = p.at("submit-file-name", default: p.name-en + "." + l.at(1))
            if (type(v) == str) {
              raw(v)
            } else if (type(v) == function){
              v(l.at(1))
            } else {
              v
            }
          })
        }).flatten()
      }
    )

    [编译选项]
    table(
      columns: columns,
      align: (left+bottom, center+bottom),
      stroke: 0.4pt,
      ..{
        compile-options.map(l => {
          if (l.len() == 2) {
            l += (1em, )
          }
          (
            text(size: l.at(2))[对于 #l.at(0) #h(1fr) 语言],
            table.cell(
              colspan: problem-list.len(),
              raw(l.at(1))
            )
          )
        }).flatten()
      }
    )
  }

  let next-problem() = {
    current-problem-idx.step()
    pagebreak()
    statement-page-begin.update(it => true)
    context {
      let problem = get-current-problem(here())
      heading(level: 1, default-problem-fullname(problem))
    }
  }

  let filename(it) = {
    set text(style: "italic", weight: "bold")
    it
  }

  let current-filename(ext) = context {
    let problem = get-current-problem(here())
    filename(problem.at("name-en") + "." + ext)
  }

  let current-sample-filename(idx, ext) = context {
    let problem = get-current-problem(here())
    filename(problem.at("name-en") + "/" + problem.at("name-en") + str(idx) + "." + ext)
  }

  let data-constraints-table-args = {
    (
      stroke: (x, y) => (
        left: if (x > 0) { .4pt },
        bottom: 2pt,
        top: if(y == 0) { 2pt } else if (y == 1) { 1.2pt } else { .4pt }
      ),
      align: center+horizon,
    )
  }
  (
    init: init,
    title: title,
    problem-table: problem-table,
    next-problem: next-problem,
    filename: filename,
    current-filename: current-filename,
    current-sample-filename: current-sample-filename,
    data-constraints-table-args: data-constraints-table-args
  )
}
