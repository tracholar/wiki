/*DEBUG
setencoding(utf-8)
*/
{% macro list_page(pages, parent_url, parent_id) -%}
    {%- for page_k, page_v in pages.items() %}
      {% if site.default_ext in page_k %}
      {"title": "{{ page_v.title }}", "text": "{{ clean_html_tag(page_v.content) }}", "tags": "{{ page_v.category }}", "url": "{{ parent_url }}/{{ page_v.name }}.html"},
      {% else %}
        {% set url = parent_url ~ "/" ~ page_k %}
        {{ list_page(page_v, url, id) }}
      {% endif %}
    {%- endfor %}
{%- endmacro %}

var tipuesearch = {"pages": [
    {% for category_name, pages in site.structure.items() %}
      {# Top-level wiki pages not display in index #}
      {% if site.default_ext not in category_name %}
      {% set url = site.root ~ "/" ~ category_name %}
        {{ list_page(pages, url, category_name) }}
      {% endif %}
    {%- endfor %}
]};
