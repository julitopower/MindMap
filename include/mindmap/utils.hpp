namespace mm {
/*! \brief Convert a mindmap file into a Graphviz digrag dot file
 *
 * \param mmfilepath The path to the mindmap file
 * \param dotfilepath The path to the output Graphviz file
 */
extern "C" bool to_dot(const char *mmfilepath, const char *dotfilepath);

/*! \brief Convert a graphviz file into a png
 *
 * \param dotfilepath The path to the Graphviz dot file
 * \param pngfilepath The path to the output png file
 */
extern "C" bool to_png(const char *dotfilepath, const char *pngfilepath);
} // namespace mm
