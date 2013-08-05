<div class="login-form-wrapper twelve columns mobile-four">
	<p class="cust-login"><?php print render($intro_text); ?></p>
	<div class="fortified-login">
  <?php print drupal_render_children($form) ?>
  </div>
</div>